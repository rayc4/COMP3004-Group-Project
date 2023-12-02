#include "sensor.h"


Sensor::Sensor(){
    //heartRate = patientRef->getHeartRate();
    //connect(&patientRef, &Patient::sendHeartRate, this, &Sensor::receiveHeartRate);
}


int Sensor::getHeartRate(){

    //heartRate = patientRef->getHeartRate();
    return heartRate;
}

void Sensor::receiveHeartRate(int hr){
    qDebug() << "heartrate in sensor class is "<< hr;
}


//Is set from GUI
void Sensor::setPlacement(bool placement){
    goodPlacement = placement;
}


//Analyzer will call this function
bool Sensor::getgoodPlacement() const{
    return goodPlacement;
}

//Is set from GUI
void Sensor::setCPRStatus(bool status){
    CPRStatus = status;
}

//Analyzer will call this function
bool Sensor::getCPRstatus() const{
    return CPRStatus;
}
