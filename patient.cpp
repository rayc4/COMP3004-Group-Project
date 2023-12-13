#include "patient.h"


Patient::Patient(int a, QString nm):age(a), name(nm)
{

    setAge(age);

    //Let patient run on it's own thread:
    patientThread = new QThread;

    moveToThread(patientThread);
    patientThread->start();


    //Initialize the patient with a resting heart rate
    randomGen.seed(QTime::currentTime().msec());

    //Patient first initialized with a regular heartbeat
    currentState = REG; //Set to regular heartbeat

    heartRateTimer = new QTimer();
    connect(heartRateTimer, &QTimer::timeout, [=]() {
        updateHeartRate();
    });
    heartRateTimer->start(1000);


    survivalTimer = new QTimer();
    connect(survivalTimer, &QTimer::timeout, this, &Patient::updateSurvivalRate);
    // Survival timer is started in setState function

    breathTime = 4000;


    breathTimer = new QTimer();
    connect(breathTimer, &QTimer::timeout, this, &Patient::breath);
    breathTimer->start(breathTime);

    //Breath timer is started in updateHeartRate() function.

}

Patient::~Patient(){
    delete heartRateTimer;
    delete patientThread;
}

void Patient::updateHeartRate(){
    //qDebug() << "-----------" << cprReset;
    QMutexLocker locker(&heartMutex);


    if (breathState == 0){
        breathTimer->stop();
        breathTime = 0;
    }
    else if (breathState == 1 && breathTime != 4000){
        breathTime = 4000;
        breathTimer->start(breathTime);
    }
    else if (breathState == 2 && breathTime != 15000){
        breathTime = 15000;
        breathTimer->start(breathTime);// Breath every 15 sec
    }

    //qDebug() << "Cpr count: " << cprCount << "Breath count: " << breathCount;

    //qDebug() << "Base survival: " << baseSurvivalChance;
    //qDebug() << "Bonus: " << survivalBonus;

    //Check with cpr affecting survival rate:
    if (cprCount >= 30 && breathCount >= 2){
        if (!forceSequence2){ //In the second sequence, CPR is ineffective.
            oneCPR = true;
            survivalBonus += 10; //If given 30 compressions + 2 breaths, survival chance goes up
            backToLife();
        }
        else{
            currentState = VTAC;
            sequence2CPRCount++;
            if (sequence2CPRCount >= 2){
                currentState = ASYS;
                sequence2CPRCount = 0;
            }
        }
        cprCount = 0;
        breathCount = 0;
    }


    if (baseSurvivalChance+survivalBonus > 100){
        baseSurvivalChance = 100;
        survivalBonus = 0;
    }

    //Too complex and unecessary
//    //There is a 5% chance that the state changes...
//    int stateChange = -1;
//    stateChange = randomGen.bounded(0, 100);
//    if (stateChange < 5){
//        if (!(currentState == REG || currentState == ASYS || currentState == CARR)) {
//            HeartState newState = static_cast<HeartState>(randomGen.bounded(1, 5));
//            currentState = newState;
//        }
//    }

    //Implement logic here to either set patient to reg, vTac, vFib, or asystole
    if (cpr)
        cpr = false;
    else if (baseSurvivalChance+survivalBonus <= 0){ //If there is no chance of survival, patient flatlines.
        currentState = ASYS;
        asystole();
    }
    else{
        //cprReset = false;
        if (currentState == REG){
            reg();
            baseSurvivalChance = 100;
            survivalBonus = 0;
        }
        else if (currentState == VTAC)
            vTac();
        else if (currentState == VFIB)
            vFib();
        else if (currentState == PEA)
            pulselessEA();
        else if (currentState == ASYS)
            asystole();
        else
            currentState = NEG;
    }

    //emit sendHeartRate(heartRate);

    //qDebug() << "Curent heartrate is" << heartRate;

    //qDebug() << "Current survival addition is " << survivalAddition;

    if (currentState == ASYS){
        baseSurvivalChance = 0;
        survivalBonus = 0;
    }


}

void Patient::updateSurvivalRate(){
    //qDebug() << "Current chances of survival decreased to" << baseSurvivalChance+survivalOffset<< "%";
    survivalTime++; //Add one second to the timer.
    if (survivalTime %60 == 0) //Every minute
        baseSurvivalChance -= 10; //Every minute, chances decrease by 10%

    if (baseSurvivalChance < 0)
        baseSurvivalChance = 0;

}

void Patient::reg(){
    survivalBonus = 0;
    cpr = false;
    oneCPR = false;
    cprCount = 0;
    breathState = 1;

    int minHR = 70;
    int maxHR = 75;

    //Generate a value from min to max heart rate
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular tachycardia
void Patient::vTac(){
    breathState = 2;

    int minHR = 240;
    int maxHR = 250;

    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular fibrillation
void Patient::vFib(){
    breathState = 2;

    int minHR = 150;
    int maxHR = 500;
    //Generate a value from 150 to 500
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient flatlines
void Patient::asystole(){
    survivalBonus = 0;
    heartRate = 0;
    breathState = 0;
}

void Patient::pulselessEA(){
    breathState = 2;

    int minHR = 1;
    int maxHR = 5;
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Chance-based on how a patient responds to a shock
void Patient::respondToShock(){
    //3 pathways: a) Regular, b) Continued vfib/vtac, or c) asystole
    //a) 50%, b) 40%, c) 10%
    int response = -1;
    if (forceSequence1){
        currentState = PEA;
        baseSurvivalChance = 90;
        qDebug() << "Setting for sequence 1";
    }
    else if (forceSequence2){
        currentState = VTAC;
        qDebug() << "Setting for sequence 2";
    }
    else if (!(currentState == ASYS)){
        response = randomGen.bounded(0, 100);
        //qDebug() << response;
        if (response > 30)
            currentState = PEA; // 30% chance that it degrades to PEA
         survivalBonus += 10; // 10% survival bonus for trying
    }

    if (!forceSequence1 || !forceSequence2)
        backToLife(); // Check to see if patient can be brought back to life

}

int Patient::getHeartRate(){
    QMutexLocker locker(&heartMutex);
    return heartRate;
}

int Patient::getAge(){
    QMutexLocker locker(&heartMutex);
    return age;
}

bool Patient::getChild(){
    QMutexLocker locker(&heartMutex);
    return isChild;
}

QString Patient::getName(){
    QMutexLocker locker(&heartMutex);
    return name;
}

int Patient::getSurvival(){
    QMutexLocker locker(&heartMutex);
    return baseSurvivalChance+survivalBonus;
    //return 100; //NOTE: This did not change internal logic for hard-code testing
}

HeartState Patient::getState(){
    QMutexLocker locker(&heartMutex);
    return currentState;
}

void Patient::setState(HeartState state){
    QMutexLocker locker(&heartMutex);
    currentState = state;
    if (currentState != REG){ //If patient heartbeat isn't regular
        if (!survivalDisabled)
            survivalTimer->start(1000);
        switch (currentState) {
            case VTAC:
                baseSurvivalChance = 30;
                break;
            case VFIB:
                baseSurvivalChance = 20;
                break;
            case ASYS:
                baseSurvivalChance = 0;
                break;
            case PEA:
                baseSurvivalChance = 10;
                break;
            default:    //Not sure how it would get to this state
                baseSurvivalChance = -1;
        }
    }
    else{
        survivalTimer->stop();
        baseSurvivalChance = 100;
        survivalBonus = 0;
    }


}


void Patient::setAge(int a){
    if (a < 8)
        isChild = true;
    else
        isChild = false;
    age = a;
}

void Patient::setName(QString n){
    name = n;
}

void Patient::receiveLeftPad(int r, int c){
    emit leftPadUpdated(r, c);
}

void Patient::receiveRightPad(int r, int c){
    emit rightPadUpdated(r, c);
}

void Patient::receiveDepth(int depth){
    emit sendDepth(depth);
}


//CPR Stuff
void Patient::patientCPS(){
    //qDebug() << "CPS";
    cpr = true;
    int cprBPM = 0;
    click++;

    if (click == 1){
        clickTime = QTime::currentTime();
        cprBPM = 60;
    }else{
        QTime currentTime = QTime::currentTime();
        int timeDifference = clickTime.msecsTo(currentTime);
        cprBPM = 60000/ timeDifference;
        clickTime = currentTime;
    }
    if (cprBPM > 80 && cprBPM < 140) //Supposed to be 100-120, but made easier here.
        cprCount++; //If its good CPR, make it count
    heartRate = cprBPM;
}

void Patient::setCPR(bool c){
    QMutexLocker locker(&heartMutex);
    cpr = c;
}

void Patient::addBreath(){
    QMutexLocker locker(&heartMutex);
    if (cprCount >= 30) //Breaths only effective after 30 compressions
        breathCount++;
}

void Patient::falseCPR(){
    cpr = false;
}

//This function looks at current survival rate and decides if the patient can come back to life
void Patient::backToLife(){
    if ((baseSurvivalChance + survivalBonus >= 100) && oneCPR)
        currentState = REG;
    else if (oneCPR && !(currentState == ASYS || currentState == REG)){ //If CPR occured at least once
        //Choose if based on the current survival chance the person can come back:
        int tempSurvival = baseSurvivalChance + survivalBonus;
        int survivalChange = 0;
        if (tempSurvival < 100){
            survivalChange = randomGen.bounded(0, 100);
            //qDebug() << "Survival change is" << survivalChange;
            //qDebug() << "temp survival is" << tempSurvival;
            if ((survivalChange <= tempSurvival) && oneCPR){
                currentState = REG;
            }
        }
    }

    //Adding a 5% chance for death here...
    if (!(currentState == REG || currentState == ASYS)) {
        int deathChange = randomGen.bounded(0, 100);
        if (deathChange < 5){
             currentState = ASYS;
        }
    }

}


void Patient::breath(){
    if (breathState == 1)
        qDebug() << "Patient breathes";
    else if (breathState == 2)
        qDebug() << "Patient breathes unsteadily";
    emit sendBreath();
}

//Only for testing purposes:
void Patient::autoCPR(){
    qDebug() << "Auto CPR called";
    cpr = true;
    heartRate = 110;
    cprCount = 30;
    breathCount = 2;
}

void Patient::setSurvivalTimer(bool enabled){
    if (enabled)
        survivalDisabled = true;
    else
        survivalDisabled = false;
}

void Patient::setSequence(int seq){
    if (seq == 1){
        forceSequence1 = true;
        forceSequence2 = false;
        setState(VFIB);
    }
    else if (seq == 2){
        forceSequence1 = false;
        forceSequence2 = true;
        setState(VTAC);
    }
    else{
        forceSequence1 = false;
        forceSequence2 = false;
    }
}
