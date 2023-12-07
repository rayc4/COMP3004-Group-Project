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

//Doesn't end up being used
//void Sensor::receiveHeartRate(int hr){
//    heartRate = hr;
//}

bool Sensor::getGoodPlacement(){
    checkPads();
    return goodPlacement;
}


int Sensor::getHeartRate(){
    if (goodPlacement)
        return pPatient->getHeartRate();
    return -1;
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
    qDebug() << "Left pad: " << leftPad;
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

    qDebug() << "Right pad: " << rightPad;
}

void Sensor::checkPads(){
    goodPlacement = leftPad && rightPad;
}

void Sensor::setDepth(int depth){
    cprDepth = depth;
}
