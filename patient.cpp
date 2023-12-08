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
    currState = 0; //Set to regular heartbeat

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
    //Implement logic here to either set patient to reg, vTac, vFib, or asystole
    if (cpr)
        cpr = false;
    else if (survivalChance == 0) //If there is no chance of survival, patient flatlines.
        asystole();
    else{
        //cprReset = false;
        if (currState == 0)
            reg();
        else if (currState == 1)
            vTac();
        else if (currState == 2)
            vFib();
        else if (currState == 3)
            asystole();
        else if (currState == 4)
            cardiacArrest();
        else
            currState = -1;
    }

    //emit sendHeartRate(heartRate);

    //qDebug() << "Curent heartrate is" << heartRate;

    //qDebug() << "Current survival addition is " << survivalAddition;
}

void Patient::updateSurvivalRate(){
    //qDebug() << "Current chances of survival decreased to" << survivalChance << "%";
    survivalTime++; //Add one second to the timer.
    if (survivalTime %60 == 0) //Every minute
        survivalChance -= 10; //Every minute, chances decrease by 10%

    if (survivalChance < 0)
        survivalChance = 0;


}

void Patient::reg(){

    int minHR = 70;
    int maxHR = 75;

    //Generate a value from min to max heart rate
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular tachycardia
void Patient::vTac(){
    int minHR = 240;
    int maxHR = 250;

    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient's body forces ventricular fibrillation
void Patient::vFib(){
    int minHR = 150;
    int maxHR = 500;
    //Generate a value from 150 to 500
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Patient flatlines
void Patient::asystole(){
    survivalChance = 0;
    heartRate = 0;
}

void Patient::cardiacArrest(){
    int minHR = 40;
    int maxHR = 50;
    heartRate = randomGen.bounded(minHR, maxHR+1);
}

//Chance-based on how a patient responds to a shock
void Patient::respondToShock(){
    //3 pathways: a) Regular, b) Continued vfib/vtac, or c) asystole
    //a) 50%, b) 40%, c) 10%
    int response = -1;
    int tempState = -1;

    response = randomGen.bounded(0, 100);
    if (response < 10 * (survivalChance / 10)) //10% chance
        tempState = 3; //asystole
    else if (response < 50 * (survivalChance / 10)) //50% chance
        tempState = currState; //Return to original state
    else //50% chance
        tempState = 0; //Return to regular heartbeat

    currState = tempState;

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

void Patient::setState(int state){
    QMutexLocker locker(&heartMutex);
    currState = state;
    if (currState != 0) //If patient heartbeat isn't regular
        survivalTimer->start(1000);
    else{
        survivalTimer->stop();
        survivalChance = 100;
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

//void leftPadUpdated(int, int){

//}

//void rightPadUpdated(int, int){

//}

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
    heartRate = cprBPM;
}

void Patient::setCPR(bool c){
    cpr = c;
}

void Patient::falseCPR(){
    cpr = false;
}


