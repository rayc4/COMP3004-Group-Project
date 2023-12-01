#include "aed.h"

AED::AED(QObject *parent, Sensor* sensor, Analyzer* analyzer)
    : QObject(parent), sensor(sensor), analyzer(analyzer), currentState(0), isChild(false), battery(0)
{

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
