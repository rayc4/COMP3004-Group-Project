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
    reg();

    heartRateTimer = new QTimer(this);
    connect(heartRateTimer, &QTimer::timeout, this, &patient::updateHeartRate);
    heartRateTimer->start(1000);  // Update heart rate every second

}


void patient::updateHeartRate(){
    //Implement logic here to either set patient to reg, vTac, vFib, or asystole


}


void patient::reg(){
    int minHR = 60;
    int maxHR = 80;

    heartRate.clear();

    for (int i = 0; i < NUM_HEARTRATES; ++i) {
            //Generate values from 60 to 80bpm which is resting heart rate
           heartRate.append(QRandomGenerator::global()->bounded(minHR, maxHR+1));
    }
}

//Patient's body forces ventricular tachycardia
void patient::vTac(){

}

//Patient's body forces ventricular fibrillation
void patient::vFib(){

}

//Patient flatlines
void patient::asystole(){

}

QList<int>& patient::getHeartRate(){
    return heartRate;
}


//Set patient's heart rate back to normal
void patient::setReg() {
    reg();
}

