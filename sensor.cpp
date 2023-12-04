#include "sensor.h"


Sensor::Sensor(){
    //Why am I here...
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


void Sensor::setPlacement(bool placement){
    goodPlacement = placement;
}

void Sensor::setDepth(int depth){
    cprDepth = depth;
}
