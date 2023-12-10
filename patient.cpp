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

    QTimer* heartRateTimer = new QTimer();
    connect(heartRateTimer, &QTimer::timeout, [=]() {
        updateHeartRate();
    });
    heartRateTimer->start(1000);


    survivalTimer = new QTimer();
    connect(survivalTimer, &QTimer::timeout, this, &Patient::updateSurvivalRate);
    // Updates timer every 1 second

}

Patient::~Patient(){
    delete heartRateTimer;
    delete patientThread;
}



void Patient::updateHeartRate(){
    //qDebug() << "-----------" << cprReset;
    QMutexLocker locker(&heartMutex);

    //qDebug() << "Cpr count: " << cprCount << "Breath count: " << breathCount;

    //qDebug() << "Base survival: " << baseSurvivalChance;
    //qDebug() << "Bonus: " << survivalBonus;

    //Check with cpr affecting survival rate:
    if (cprCount >= 30 && breathCount >= 2){
        oneCPR = true;
        cprCount = 0;
        breathCount = 0;
        survivalBonus += 10; //If given 30 compressions + 2 breaths, survival chance goes up
        backToLife();
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
    else if (baseSurvivalChance+survivalBonus <= 0) //If there is no chance of survival, patient flatlines.
        asystole();
    else{
        //cprReset = false;
        if (currentState == REG)
            reg();
        else if (currentState == VTAC)
            vTac();
        else if (currentState == VFIB)
            vFib();
        else if (currentState == CARR)
            cardiacArrest();
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
    //qDebug() << "Current chances of survival decreased to" << survivalChance << "%";
    survivalTime++; //Add one second to the timer.
    if (survivalTime %60 == 0) //Every minute
        baseSurvivalChance -= 10; //Every minute, chances decrease by 10%

    if (baseSurvivalChance < 0)
        baseSurvivalChance = 0;


}

void Patient::reg(){
    baseSurvivalChance = 100;
    survivalBonus = 0;
    cpr = false;
    oneCPR = false;
    cprCount = 0;

    int minHR = 70;
    int maxHR = 75;

    //Generate a value from min to max heart rate
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular tachycardia
void Patient::vTac(){
    baseSurvivalChance = 40;

    int minHR = 240;
    int maxHR = 250;

    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular fibrillation
void Patient::vFib(){
    baseSurvivalChance = 30;

    int minHR = 150;
    int maxHR = 500;
    //Generate a value from 150 to 500
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient flatlines
void Patient::asystole(){
    baseSurvivalChance = 0;
    heartRate = 0;
}

void Patient::cardiacArrest(){
    baseSurvivalChance = 20;

    int minHR = 40;
    int maxHR = 50;
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Chance-based on how a patient responds to a shock
void Patient::respondToShock(){
    //3 pathways: a) Regular, b) Continued vfib/vtac, or c) asystole
    //a) 50%, b) 40%, c) 10%
    int response = -1;
    if (!(currentState == ASYS)){
        response = randomGen.bounded(0, 100);
        //qDebug() << response;
        if (response < 70) { // 70% chance that it fails and falls back to original survival chance
            //Do nothing
            survivalBonus += 10; // 10% survival bonus for trying
        }
        else // 30% chance patient comes back to life
            baseSurvivalChance = 100; //Patient lives!
    }

    backToLife();

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
}

HeartState Patient::getState(){
    QMutexLocker locker(&heartMutex);
    return currentState;
}

void Patient::setState(HeartState state){
    QMutexLocker locker(&heartMutex);
    currentState = state;
    if (currentState != REG) //If patient heartbeat isn't regular
        survivalTimer->start(1000);
    else{
        survivalTimer->stop();
        baseSurvivalChance = 100;
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
            qDebug() << "Survival change is" << survivalChange;
            qDebug() << "temp survival is" << tempSurvival;
            if ((survivalChange <= tempSurvival) && oneCPR){
                currentState = REG;
            }
        }
    }

}


//Only for testing purposes:
void Patient::autoCPR(){
    qDebug() << "Auto CPR called";
    cpr = true;
    heartRate = 110;
    cprCount = 30;
    breathCount = 2;
}
