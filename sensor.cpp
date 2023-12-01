#include "sensor.h"


sensor::sensor(){
}

//Getters
int sensor::getHeartRate() const{
    return heartRate;
}

bool sensor::getCPRstatus() const{
    return CPRstatus;
}

bool sensor::getgoodPlacement() const{
    return goodPlacement;
}
