#include "patient.h"

patient::patient(bool iC, QString nm): isChild(iC), name(nm)
{
    //Let patient run on it's own thread:
    QThread* patientThread = new QThread;

    moveToThread(patientThread);
    patientThread->start();


    //Initialize the patient with a resting heart rate
    QRandomGenerator::global()->seed(QTime::currentTime().msec());

    //Patient first initialized with a regular heartbeat
    currState = 0; //Set to regular heartbeat

    QTimer* heartRateTimer = new QTimer();
    connect(heartRateTimer, &QTimer::timeout, [=]() {
        updateHeartRate();
    });
    heartRateTimer->start(1000);

}



void patient::updateHeartRate(){
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
}


void patient::reg(){

    int minHR = 70;
    int maxHR = 75;

    //Generate a value from min to max heart rate
    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));


}

//[70,74,76,80,...]


//Patient's body forces ventricular tachycardia
void patient::vTac(){
    int minHR = 240;
    int maxHR = 250;

    //Generate a value from 150 to 250
    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));
}

//Patient's body forces ventricular fibrillation
void patient::vFib(){
    int minHR = 150;
    int maxHR = 500;

    //Generate a value from 150 to 500
    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));
}

//Patient flatlines
void patient::asystole(){
    heartRate = 0;
}

int patient::getHeartRate(){
    QMutexLocker locker(&heartMutex);
    return heartRate;
}

void patient::setState(int state){
    QMutexLocker locker(&heartMutex);
    currState = state;
}
