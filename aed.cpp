#include "aed.h"
#include <QDebug>
#include <QCoreApplication>

// stage functions (in order) ====================================

void AED::checkResponsiveness()
{
    communicateWithUser("Check responsiveness of the patient.");
    waitTimer->start(WAIT_MS);
}

void AED::callEmergencyServices()
{
    communicateWithUser("Call emergency services.");
    waitTimer->start(WAIT_MS);
}

void AED::tiltHead(){
    communicateWithUser("Tilt patient's head.");
    waitTimer->start(WAIT_MS);
}

void AED::checkAirway()
{
    communicateWithUser("Check for breathing and airway of patient.");
    waitTimer->start(WAIT_MS);
}

void AED::attachDefibPad()
{
    if (!pSensor->getGoodPlacement()){
        communicateWithUser("Bad pad placement. Waiting for good placement...");
    }
    while(!pSensor->getGoodPlacement()){
        QCoreApplication::processEvents();
    }
    emit stageComplete();
}

void AED::standClear(){
    qDebug() << "[SPEAKER] Stand clear. Schock commencing.";
    updateText("Stand clear. Schock commencing.");
    waitTimer->start(WAIT_MS);
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
//    QString feedback;
//    pAnalyzer->checkCPR(pSensor->getDepth(), pSensor->getChild(), feedback);
//    qDebug() << feedback;

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
                      &AED::tiltHead,
                      &AED::checkAirway,
                      &AED::attachDefibPad,
                      &AED::standClear,
                      &AED::instructCPR};

    waitTimer = new QTimer(this);
    waitTimer->setSingleShot(true);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(enterNextState()));

    connect(this, SIGNAL(stageComplete()), this, SLOT(enterNextState()));

    //Zuhayr
    updateTimer = new QTimer();
    connect(updateTimer, &QTimer::timeout, [=]() {
        updateAED();
    });
    updateTimer->start(300);
    // Zuhayr
}

AED::~AED(){

}

void AED::communicateWithUser(std::string const & s){
    QString qs = QString::fromStdString("[SPEAKER] " + s);
    qDebug() << qs;
    updateText(s);
}

void AED::checkForShock()
{
    int heartState = pAnalyzer->analyzeHeart();

    // Heart states: 0 - Regular, 1 - Vtac, 2 - Vfib, 3 - Asystole, 4 - Unknown
    //TODO: talk to the others, pls i dont want switch case
    //Switch case implemented so it can mimic Zuhayr and Justin design thought process

     switch(heartState) {
         case 1: // Vtac
            qDebug() << "[SPEAKER] Shock Advised. Preparing to shock.";
            prepareForShock(0);
            stageComplete();
            break;
         case 2: // Vfib
             qDebug() << "[SPEAKER] Shock Advised. Preparing to shock.";
             prepareForShock(0);
             stageComplete();
             break;
         case 0: // Regular
            qDebug() << "[SPEAKER] Regular heartbeat.";
            stageComplete();
            break;
         case 3: // Asystole
            qDebug() << "[SPEAKER] Asystole heartbeat.";
            stageComplete();
            break;
         case 4: // Unknown
            qDebug() << "[SPEAKER] stuff broken, unkown heartbeat.";
            stageComplete();
            break;
         default:
             qDebug() << "[SPEAKER] No shock advised.";
             stageComplete();
             break;
     }
}

//Function added by Zuhayr
void AED::updateAED(){
    pAnalyzer->CollectHeart(pSensor->getHeartRate());
    QString feedback;
    pAnalyzer->checkCPR(pSensor->getDepth(), pSensor->getChild(), feedback);
    //qDebug() << feedback;
}


void AED::enterNextState(){
    if(state == FINAL_STATE){
        return;
    }
    updateState(++state);
    (this->*(stateFunctions[state]))();
    //this-> compile issue
    //this->* is definetly derreferring the iterator
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
    //goodCPR = pAnalyzer->checkCPR(pSensor->getHeartRate());
    if (goodCPR){
        qDebug() << "GOOD";
    }else{
        qDebug() << "BAD"; //TODO: come back here later
    }
}

Sensor* AED::getSensor(){
    return pSensor;
}



//this is a comment

