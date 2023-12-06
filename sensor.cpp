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

bool Sensor::getgoodPlacement(){
    checkPads();
    return goodPlacement;
}


int Sensor::getHeartRate(){
    return heartRate;
}

int Sensor::getDepth() const{
    return cprDepth;
}


void Sensor::setLPlacement(int row,int column){
    //Threshold (+/-1) from row 5
    //Threshold (+/-2) from column 7
    if ((row >= 4 and row <= 6) && (column >= 6 and column <= 8)){
        leftPad = true;
    }else{
        leftPad = false;
    }

}

void Sensor::setRPlacement(int row,int column){
//    Threshold (+/-1) from row 10
//    Threshold (+/-1) from column 4
    if ((row >= 9 and row <= 11) && (column >= 3 and column <= 5)){
        rightPad = true;
        checkPads();
    }else{
        rightPad =false;
        checkPads();
    }
}

void Sensor::checkPads(){
    if (leftPad && rightPad){
        goodPlacement = true;
    }else{
        goodPlacement = false;
    }
}

void Sensor::setDepth(int depth){
    cprDepth = depth;
}
