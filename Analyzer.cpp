#include "Analyzer.h"


Analyzer::Analyzer(QObject *parent) : QObject(parent), sensor(nullptr), heartState(0), shockState(false) {
}

Analyzer::~Analyzer() {

    delete sensor;
}

int Analyzer::analyzeHeart() {

    return heartState; // Placeholder 
}

bool Analyzer::CPRFeedback(const std::string& feedback) {
    return true; // Placeholder 
}

void Analyzer::setShock(bool shock) {
    shockState = shock;
}

bool Analyzer::checkPlacement() {
    return true; // Placeholder 
}

// Getters
Sensor* Analyzer::getSensor() const {
    return sensor;
}

int Analyzer::getHeartState() const {
    return heartState;
}

bool Analyzer::getShockState() const {
    return shockState;
}

// Setters
void Analyzer::setSensor(Sensor* newSensor) {
    if (sensor != newSensor) {
        delete sensor; 
        sensor = newSensor;
    }
}

void Analyzer::setHeartState(int state) {
    heartState = state;
}

void Analyzer::setShockState(bool state) {
    shockState = state;
}
