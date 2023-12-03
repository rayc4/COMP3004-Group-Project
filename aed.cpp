#include "aed.h"

AED::AED(QObject *parent)
    : QObject(parent)
{
    sensor = new Sensor();
    analyzer = new Analyzer();
}

AED::~AED(){

}

void AED::updateAED()
{

}

void AED::powerOn()
{

}

bool AED::guidePlacement(bool isChild)
{
    // true or false
    return true; // Placeholder
}

bool AED::readyToShock()
{
    //  true or false
    return true; // Placeholder
}

void AED::setMessage(const std::string& audioMessage, const std::string& displayMessage)
{

}

void AED::receiveSensorData(int data){

}


//ONLY CALL THIS FUNCTION WHEN YOU WANT TO CHECK CPR (LOGIC MISSING)
void AED::determineCPRStatus(){
    goodCPR = analyzer->checkCPR(sensor->getHeartRate());
    if (goodCPR){
        qDebug() << "GOOD";
    }else{
        qDebug() << "BAD";
    }
}

Sensor* AED::getSensor() const{
    return sensor;
}

