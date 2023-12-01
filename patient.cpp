#include "patient.h"

Patient::Patient(bool iC, QString nm): isChild(iC), name(nm)
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
}


void Patient::reg(){

    int minHR = 70;
    int maxHR = 75;

    //Generate a value from min to max heart rate
    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));


}

//[70,74,76,80,...]


//Patient's body forces ventricular tachycardia
void Patient::vTac(){
    int minHR = 240;
    int maxHR = 250;

    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));
}

//Patient's body forces ventricular fibrillation
void Patient::vFib(){
    int minHR = 150;
    int maxHR = 500;

    //Generate a value from 150 to 500
    heartRate = (QRandomGenerator::global()->bounded(minHR, maxHR+1));
}

//Patient flatlines
void Patient::asystole(){
    heartRate = 0;
}

int Patient::getHeartRate(){
    QMutexLocker locker(&heartMutex);
    return heartRate;
}

void Patient::setState(int state){
    QMutexLocker locker(&heartMutex);
    currState = state;
}
