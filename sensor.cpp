#include "sensor.h"


Sensor::Sensor(){
    //heartRate = patientRef->getHeartRate();
    //connect(&patientRef, &Patient::sendHeartRate, this, &Sensor::receiveHeartRate);
}

void Sensor::receiveHeartRate(int hr){
    heartRate = hr;
}


//Is set from GUI
void Sensor::setPlacement(bool placement){
    goodPlacement = placement;
}

////Is set from GUI
//void Sensor::setCPRStatus(bool status){
//    CPRStatus = status;
//    qDebug() << "Current Status: " << CPRStatus;
//}

//Analyzer will call this function
bool Sensor::getgoodPlacement() const{
    return goodPlacement;
}

////Analyzer will call this function
//bool Sensor::getCPRstatus() const{
//    return CPRStatus;
//}

int Sensor::getHeartRate(){
    //heartRate = patientRef->getHeartRate();
    return heartRate;
}

//void Sensor::setCPR(int cBpm){
//    cprBPM = cBpm;
//}

//int Sensor::getCPR() const{
//    return cprBPM;
//}

