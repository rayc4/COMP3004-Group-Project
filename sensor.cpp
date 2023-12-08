#include "sensor.h"


Sensor::Sensor(Patient* aPatient): pPatient(aPatient)
{
    //Why am I here...
}


void Sensor::setPatient(Patient* tempPatient)
{
    if(!tempPatient) return;
    pPatient = tempPatient;
    connect(pPatient, SIGNAL(leftPadUpdated(int,int)), this, SLOT(setLPlacement(int,int)));
    connect(pPatient, SIGNAL(rightPadUpdated(int,int)), this, SLOT(setRPlacement(int,int)));
    connect(pPatient, SIGNAL(sendDepth(int)), this, SLOT(setDepth(int)));
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

void Sensor::setLPlacement(int row,int column){
    //Threshold (+/-1) from row 5
    //Threshold (+/-2) from column 7
    if ((row >= 4 and row <= 7) && (column >= 4 and column <= 6)){
        leftPad = true;
        checkPads();
    }else{
        leftPad = false;
        checkPads();
    }
}

void Sensor::setRPlacement(int row,int column){
//    Threshold (+/-1) from row 10
//    Threshold (+/-1) from column 4
    if ((row >= 9 and row <= 13) && (column >= 2 and column <= 5)){
        rightPad = true;
        checkPads();
    }else{
        rightPad =false;
        checkPads();
    }
}

void Sensor::checkPads(){
    goodPlacement = leftPad && rightPad;
}

void Sensor::setDepth(int depth){
    cprDepth = depth;
    pressureCPR = true;
    qDebug() << cprDepth;
}

int Sensor::getDepth(){
    return cprDepth;
}

bool Sensor::getChild(){
    return pPatient->getChild();
}

