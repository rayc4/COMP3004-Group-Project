#include "patient.h"


Patient::Patient(bool iC, QString nm):isChild(iC), name(nm)
{


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
    heartRateTimer->start(300);


    QTimer* survivalTimer = new QTimer();
    // Connect the timeout signal to your slot
    connect(survivalTimer, &QTimer::timeout, this, &Patient::updateSurvivalRate);
    // Start the timer (e.g., every 1000 milliseconds for seconds)
    survivalTimer->start(1000);

}

Patient::~Patient(){
    delete heartRateTimer;
    delete patientThread;
}



void Patient::updateHeartRate(){

    QMutexLocker locker(&heartMutex);
    //Implement logic here to either set patient to reg, vTac, vFib, or asystole
    if (currState == 0)
        reg();
    else if (currState == 1)
        vTac();
    else if (currState == 2)
        vFib();
    else if (currState == 3)
        asystole();
    else
        currState = -1;
    emit sendHeartRate(heartRate);

    //qDebug() << "Curent heartrate is" << heartRate;

    //qDebug() << "Current survival addition is " << survivalAddition;
}

void Patient::updateSurvivalRate(){
    survivalTime++; //Add one second to the timer.
    if (survivalTime %60 == 0) //Every minute
        survivalAddition += 10; //Every minute, chances decrease by 10%

    if (survivalAddition > 100)
        survivalAddition = 100;


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
    heartRate = 0;
}

//Chance-based on how a patient responds to a shock
void Patient::respondToShock(){
    //3 pathways: a) Regular, b) Continued vfib/vtac, or c) asystole
    //a) 50%, b) 40%, c) 10%
    int response = -1;
    int tempState = -1;

    response = randomGen.bounded(0, 100-survivalAddition);
    if (response < 50) //50% chance
        tempState = 0; //Return to regular heartbeat
    else if (response < 90) //40% chance
        tempState = currState; //Keep the original state
    else //10% chance
        tempState = 3; //Set to asystole

    currState = tempState;

}

int Patient::getHeartRate(){
    QMutexLocker locker(&heartMutex);
    return heartRate;
}

void Patient::setState(int state){
    QMutexLocker locker(&heartMutex);
    currState = state;
}

//CPR Stuff
void Patient::patientCPS(){
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
//        qDebug() << cprBPM;
    }
    emit sendBPM(cprBPM);
}

