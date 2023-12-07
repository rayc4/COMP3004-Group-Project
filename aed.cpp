#include "aed.h"
#include <QDebug>
#include <QCoreApplication>

// state functions (in order) ====================================

void AED::checkResponsiveness()
{
    qDebug() << "[SPEAKER] Check responsiveness of the patient.";
    updateText("Check responsiveness of the patient.");
    waitTimer->start(WAIT_MS);
}

void AED::callEmergencyServices()
{
    qDebug() << "[SPEAKER] Call emergency services.";
    updateText("Call emergency services.");
    waitTimer->start(WAIT_MS);
}

void AED::checkAirway()
{
    qDebug() << "[SPEAKER] Check for breathing and airway of patient.";
    updateText("Check for breathing and airway of patient.");
    waitTimer->start(WAIT_MS);
}

void AED::attachDefibPad()
{
    if (!sensor->getGoodPlacement()){
        qDebug() << "[SPEAKER] Bad pad placement. Waiting for good placement...";
    }
    while(!sensor->getGoodPlacement()){
        QCoreApplication::processEvents();
    }
    stageComplete();
}

void AED::checkForShock()
{
    int heartState = pAnalyzer->analyzeHeart();

    // Heart states: 0 - Regular, 1 - Vtac, 2 - Vfib, 3 - Asystole, 4 - Unknown
    //TODO: talk to the others, pls i dont want switch case
    //Switch case implemented so it can mimic Zuhayr and Justin design thought process

     switch(heartState) {
         case 1: // Vtac
         case 2: // Vfib
             qDebug() << "[SPEAKER] Shock Advised. Preparing to shock.";
             prepareForShock(0);
             stageComplete();
             break;
         case 0: // Regular
         case 3: // Asystole
         case 4: // Unknown
         default:
             qDebug() << "[SPEAKER] No shock advised.";
             stageComplete();
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
}

void AED::checkAirBreathing()
{
    //function 7
    qDebug()<< "Check if the patient is breathing";
    // Logic for check breathing
}

// end of state functions =============================

AED::AED(QObject *parent)
    : QObject(parent)
{
    pSensor = new Sensor();
    pAnalyzer = new Analyzer();

    stateFunctions = {&AED::checkResponsiveness,
                      &AED::callEmergencyServices,
                      &AED::checkAirway,
                      &AED::attachDefibPad,
                      &AED::checkForShock,
                      &AED::instructCPR,
                      &AED::checkAirBreathing};

    waitTimer = new QTimer(this);
    waitTimer->setSingleShot(true);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(enterNextState()));

    connect(this, SIGNAL(stageComplete()), this, SLOT(enterNextState()));
}

AED::~AED(){

}

void AED::enterNextState(){
    if(state == FINAL_STATE){
        return;
    }
    updateState(++state);
    (this->*(stateFunctions[state]))();
}

void AED::power()
{
    if(state != -1){
        state = -1;
        updateText("");
    }else{
        updateText("Powered on, performing self-check");
        waitTimer->start(WAIT_MS);
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
    qDebug() << pAnalyzer;
    goodCPR = pAnalyzer->checkCPR(pSensor->getHeartRate());
    if (goodCPR){
        qDebug() << "GOOD";
    }else{
        qDebug() << "BAD";
    }
}

Sensor* AED::getSensor(){
    return pSensor;
}



//this is a comment

