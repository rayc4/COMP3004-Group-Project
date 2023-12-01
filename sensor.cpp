#include "sensor.h"


Sensor::Sensor(Patient& p):patientRef(p){
    heartRate = patientRef.getHeartRate();
}


int Sensor::getHeartRate(){
    return heartRate;
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
