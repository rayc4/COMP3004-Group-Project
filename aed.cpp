#include "aed.h"
#include <QDebug>

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
        attachDefibPad(); // l

}
void AED::attachDefibPad()
{
    // TODO: get padPlacement =============================================

    bool padPlacement = true;

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

void test(){

}
AED::AED(QObject *parent)
    : QObject(parent)
{
    sensor = new Sensor();
    analyzer = new Analyzer();

    stateFunctions.push_back(&AED::checkResponsiveness);

//    functions.insert(functions.end(),
//                      {&AED::checkResponsiveness,
//                       &AED::callEmergencyServices,
//                       &AED::checkAirway,
//                       &AED::attachDefibPad,
//                       &AED::checkForShock,
//                       &AED::instructCPR,
//                       &AED::checkAirBreathing
//                      });
}

AED::~AED(){

}

void AED::updateAED()
{
    for(FuncVector::iterator i = stateFunctions.begin(); i != stateFunctions.end(); i++) {
        (*i)();
    }
}

void AED::power()
{
    if(state != -1){
        state = -1;
        updateText("");
    }else{
        state = 0;
        updateText("Powered On");
        // TODO: "self-check"
        updateAED();
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

int AED::getState() const{
    return state;
}

void AED::setState(int state){
    this->state = state;
    updateState(state);
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
