#include "aed.h"
#include <QDebug>
#include <QCoreApplication>

// stage functions (in order) ====================================

void AED::checkResponsiveness()
{
    if (pSensor->getGoodPlacement()){
        communicateWithUser("Pad has already been placed, skipping steps...");
        state = 4;
        waitTimer->start(WAIT_MS);
    }else{
        communicateWithUser("Check responsiveness of the patient.");
        waitTimer->start(WAIT_MS);
    }

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
    communicateWithUser("Good placement detected. Analyzing heart rate. Stand back.");
    waitTimer->start(WAIT_MS);
    emit stageComplete();
}

void AED::standClear()
{
    communicateWithUser("Stand clear of the patient!!");
    HeartState heartState = pAnalyzer->analyzeHeart();
    qDebug() << heartState;
    if(heartState == NEG)
    {
        repeatCurrentState();
        //standClear();
        return;
    }
    // Heart states: REG - Regular, VTAC - Vtac, VFIB - Vfib, PEA - PEA, ASYS - Asystole, NEG - Unknown
    switch(heartState) {
        case REG: // Regular
            communicateWithUser("Regular heartbeat.");
            qDebug() << "[SPEAKER] Regular heartbeat.";
            break;
        case VTAC: // Vtac
            communicateWithUser("Patient in V-TAC.");
            qDebug() << "[SPEAKER] Shock Advised. Preparing to shock.";
            prepareForShock();
            break;
        case VFIB: // Vfib
            communicateWithUser("Patient in V-FIB.");
            qDebug() << "[SPEAKER] Shock Advised. Preparing to shock.";
            prepareForShock();
            break;
        case ASYS: // Asystole
            communicateWithUser("Patient is ASYS.");
            qDebug() << "[SPEAKER] Asystole heartbeat.";
            break;
        case PEA:
            communicateWithUser("Patient in PEA.");
            qDebug() << "[SPEAKER] No pulse, electrical actvity detected.";
            updateState(6);
            (this->*(stateFunctions[6]))();
            break;
        case NEG: // Unknown
            communicateWithUser("Cannot determine patient status.");
            qDebug() << "[SPEAKER] Unknown heartbeat. Unable to advise.";
            break;
        default:
            communicateWithUser("No shock advised.");
            qDebug() << "[SPEAKER] No shock advised.";
            break;
    }
    if (!(heartState == VTAC || heartState == VFIB))
        emit stageComplete();
}

void AED::instructCPR()
{

    HeartState currState = pAnalyzer->getHeartState();
    //function 6
    if(currState != REG && currState !=ASYS){
        if (cprCheckCount == 0){
            qDebug()<< "Instructing CPR!!!";
            communicateWithUser("Perform CPR.");
        }
        pAnalyzer->checkCPR(pSensor->getDepth(), pSensor->getChild(), cprFeedback);
        if (cprFeedback != lastFeedback){
            communicateWithUser(cprFeedback.toStdString());
            lastFeedback = cprFeedback;
        }
        if (cprCheckCount < 30){
            cprCheckCount++;
            QTimer::singleShot(900, this, &AED::instructCPR);
        }
        else{
            cprCheckCount = 0;
            state=5;
            standClear();
        }
    }
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
                      &AED::instructCPR,
                      &AED::endState};

    waitTimer = new QTimer(this);
    waitTimer->setSingleShot(true);
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(enterNextState()));

    connect(this, SIGNAL(stageComplete()), this, SLOT(enterNextState()));

    updateTimer = new QTimer();
    connect(updateTimer, &QTimer::timeout, [=]() {
        updateAED();
    });
    updateTimer->start(300);

    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, [=](){
        batteryUpdate();
    });
    batteryTimer->start(15000);
}

AED::~AED(){

}

void AED::prepareForShock() {
    if (charging == false)
        chargeLevel = 0;
    incrementCharge(); // Start the charging process
}

void AED::incrementCharge() {
    chargeLevel += 10;
    if (chargeLevel >= 100) {
        chargeLevel = 100;
        communicateWithUser("Charge ready. Press the shock button now.");
        qDebug() << "Charge ready. Press the shock button now.";
        charging = false;
        emit doneCharging();
    } else {
        charging = true;
        communicateWithUser("CHARGING: %"+std::to_string(chargeLevel));
        qDebug() << "CHARGING: " << chargeLevel << "%";
        repeatCurrentState();
    }
}


void AED::communicateWithUser(std::string const & s){

    QString qs = QString::fromStdString("[SPEAKER] " + s);
    if (qs != lastFeedback){
        qDebug() << qs;
        updateText(s);
        lastFeedback = qs;
    }

}

void AED::batteryUpdate(){
    if (battery > 0){
        battery --;
        batteryLogic(battery);
    }
}

void AED::updateAED() {
    pAnalyzer->CollectHeart(pSensor->getHeartRate());

    HeartState currentHeartState = pAnalyzer->analyzeHeart();

    // Check for changes in heart state
    if (currentHeartState != prevHeartState) {
        qDebug() << "Heart state changed!";
        prevHeartState = currentHeartState;
        if (state > 5 && cprCheckCount >= 30) {
            updateState(5);
            state = 5;
            standClear();
        }
        if (currentHeartState == REG && state > 4){
            updateText("Patient in stable condition.");
        }
    }

    if (currentHeartState == ASYS){
        updateText("Patient has flatlined.");
        state = 7;
        endState();
        return;
    }

    if (!pSensor->getGoodPlacement() && state > 4){
        updateState(4);
        state = 4;
        attachDefibPad();
    }

}



void AED::enterNextState(){
    if(state == FINAL_STATE) return;
    updateState(++state);
    (this->*(stateFunctions[state]))();
    //this-> compile issue
    //this->* is definetly derreferring the iterator
}


void AED::repeatCurrentState()
{
    //we are calling this when we need to repeat a function
    //usually because we are waiting for something else to be done running
    //or to run 2 things on our system one after the other
    //this sets a timer for 300
    //allowing other things to update, or be process
    //then check the function again
    if(state == FINAL_STATE) return;

    // Disconnect any existing connections to ensure only the repeat function is called
    disconnect(waitTimer, SIGNAL(timeout()), this, SLOT(enterNextState()));
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(repeatFunction()));

    waitTimer->start(300); // Start the timer for 300 milliseconds
}

void AED::repeatFunction() {
    disconnect(waitTimer, SIGNAL(timeout()), this, SLOT(repeatFunction()));
    connect(waitTimer, SIGNAL(timeout()), this, SLOT(enterNextState()));

    (this->*(stateFunctions[state]))();  // Call the current function again
}



void AED::power()
{
    if (battery <= 0){
        updateText("Please Charge. There is no battery");
    }else if (battery < 16){
        updateText("Please Charge. Low Battery");
    }else{
        if (state != -1) {
            updateText("Machine is now off.");
            state = -1; // Update the state to reflect that the machine is off
            updateTimer->stop(); // Stop the timer
        } else {
            // Machine is off
            updateText("Powered on, performing self-check");
            state = 0; // Update the state to reflect that the machine is on
            waitTimer->start(WAIT_MS);
            updateTimer->start(300); // Start updating when powered on
        }
    }

}

int AED::getState() const{
    return state;
}

void AED::setState(int state){
    this->state = state;
    updateState(state);
}

Sensor* AED::getSensor(){
    return pSensor;
}

void AED::batteryLogic(int value){
    battery = value;
    updateBattery(battery);
}

void AED::setShockPressed(){
    if (chargeLevel >= 100) {
        // Logic to handle the shock delivery
        qDebug() << "Delivering shock...";
        pSensor->sendShock();
        battery -= 20;
        emit updateBattery(battery);
        emit stageComplete();
    } else {
        qDebug() << "Charge not ready. Current charge level: " << chargeLevel << "%";
    }
}

//This function generates intervals for the ecg graph
float AED::generateInterval() {
    /*Logic for resting heartrate:
        If we assume it generates a BPM between 70-70bpm, that's...
        60/70 ~ 0.85
        60/75 ~ 0.8
        Between every 0.8-0.85s, it should send one heartbeat.
        Because updateGraph generates 7 points, we need to divide this by 7.
        (0.8/7)*100 ~ 115
        (0.5/7)*100 ~ 121
        I multipled it by 1000 to convert it to ms.
        Therefore, we need to call updateGraph to show pulse every 115-121ms...

    */
    HeartState currentState = pAnalyzer->analyzeHeart();
    int bpm;
    if (currentState == REG){
        bpm = pSensor->getHeartRate();
        if (bpm < 0)
            return 1000;
        return 1000*((60.0/bpm)/7.0);
    }
    else if (currentState == VTAC || currentState == VFIB){
        bpm = pSensor->getHeartRate();
        return 1000*((60.0/bpm)/3.0);
    }
    else if (currentState == ASYS || currentState == PEA){
        return 1000*((60.0/250)/3.0); //not actual bpm, but how often it updates
    }

    return 1;
}



Analyzer* AED::getAnalyzer(){
    return pAnalyzer;
}

void AED::endState(){
    communicateWithUser("End of treatment.");
}
