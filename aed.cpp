#include "aed.h"

AED::AED(QObject *parent)
    : QObject(parent)
{
    sensor = new Sensor();

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
    //Do nothing, for now...
}


Sensor* AED::getSensor() const{
    return sensor;
}
