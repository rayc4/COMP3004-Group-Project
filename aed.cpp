#include "aed.h"
#include <QDebug>

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
//UPDATE HERE
    //THIS CAN MIMIC UNITY UPDATE
    //either this kickstart the checkers
    //or
    //step on step runs here


    //option 1: options have returns that unless met, we keep calling from here

    //option 2: here we run on a start point until the user has settle which step start at (1 or shock)
}

void AED::power()
{
    if(poweredOn){
        poweredOn = false;
        updateText("");
    }else{
        poweredOn = true;
        updateText("Powered On");
    }
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
    qDebug() << analyzer;
    goodCPR = analyzer->checkCPR(sensor->getHeartRate());
    if (goodCPR){
        qDebug() << "GOOD";
    }else{
        qDebug() << "BAD";
    }
}

Sensor* AED::getSensor(){
    return sensor;
}


//AED steps logic


void AED::checkResponsiveness()
{
//function 1
    qDebug() << "Check responsiveness of the patient.";
    // Next step
    callEmergencyServices();
}
void AED::callEmergencyServices()
{
    //function 2
    qDebug()  << "Call emergency services.";
    // Next step
    checkAirway();
}
void AED::checkAirway()
{
    //function 3
    qDebug() << "Check for breathing and airway of patient";
        // Retry || bypass to different step
        attachDefibPad(/* padPlacement */ true); // l

}
void AED::attachDefibPad(bool padPlacement)
{
    //function 4
    if (padPlacement)
    {
        //next step
        checkForShock();
    } else {
        qDebug() << "Try again place the pad correctly";
        // Retry || bypass to different step
    }
}
void AED::checkForShock()
{
    //function 5
    int heartState = analyzer->analyzeHeart();

     // Heart states: 0 - Regular, 1 - Vtac, 2 - Vfib, 3 - Asystole, 4 - Unknown
    //TODO: talk to the others, pls i dont want switch case
    //Switch case implemented so it can mimic Zuhayr and Justin design thought process

     switch(heartState) {
         case 1: // Vtac
         case 2: // Vfib
             qDebug() << "Shock Advised. Preparing to shock.";
             prepareForShock(0);
             break;
         case 0: // Regular
         case 3: // Asystole
         case 4: // Unknown
         default:
             qDebug() << "No shock advised.";
             instructCPR(); //CPR instructions
             break;
     }
}


void AED::prepareForShock(int i)
{
    //function 5.5
    //function 5 helper
    if(i>=100)
    {
    qDebug() << "Charge ready";
    qDebug() << "Press the shock button now.";
    return;
    }
    // Simulate battery charging
    qDebug() << "CHARGING";
    prepareForShock(i+1);

    // simulate pressing shock button
    // After pressing button, call deliverShock()
}


void AED::instructCPR()
{
    //function 6

    qDebug()<< "Instructing CPR!!!";
    // CPR instruction here
    //CPR quality here

    // Retry || bypass to different step
    checkAirBreathing();
}

void AED::checkAirBreathing()
{
    //function 7
    qDebug()<< "Check if the patient is breathing";
    // Logic for check breathing
}
