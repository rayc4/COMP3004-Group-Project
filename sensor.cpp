#include "sensor.h"


Sensor::Sensor(Patient* aPatient): pPatient(aPatient)
{
    //Why am I here...
}


void Sensor::setPatient(Patient* tempPatient)
{
    if(tempPatient)
    {
        pPatient = tempPatient;
    }
}


void Sensor::receiveHeartRate(int hr){
    heartRate = hr;
}

bool Sensor::getgoodPlacement() const{
    return goodPlacement;
}


int Sensor::getHeartRate(){
    return heartRate;
}

int Sensor::getDepth() const{
    return cprDepth;
}


void Sensor::setPlacement(int r,int c){
    qDebug() << "Test";
    qDebug() << r;
    qDebug() << c;
}

void Sensor::setDepth(int depth){
    cprDepth = depth;
}
