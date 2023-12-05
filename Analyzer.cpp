#include "Analyzer.h"


Analyzer::Analyzer(QObject *parent) : QObject(parent), sensor(nullptr), heartState(0), shockState(false){
}

Analyzer::~Analyzer() {

    delete sensor;
}

void Analyzer::CollectHeart(int hbit)
{
    if(heartbits.size() >= 120)
    {
        heartbits.pop_front();
    }
    heartbits.push_back(hbit);
}

int Analyzer::analyzeHeart()
{
    if(heartbits.empty() || (heartbits.size() <=60))
    {
        return -1;
    }


    int sum = 0;
    int minBit = 0;
    int maxBit = 360;

    bool isErratic = false;

    int previousBeat = heartbits.front();
    for(int i = 0; i < heartbits.size(); i++)
    {
        int beat = heartbits.at(i);
        sum += beat;

        //double if in case of flat line

        if(beat < minBit) minBit = beat;
        if(beat > maxBit) maxBit = beat;

        if(abs(beat - previousBeat) >30)
        {
            isErratic = true;
        }
        previousBeat= beat;

    }

    double average = sum / static_cast<double>(heartbits.size());

    // Detect conditions
    if (average < 30) {
        heartState=3; //testing for the lads
        return 3;
        //"Asystole" Very low or no heart rate
    } else if (average > 100) {
        if (isErratic) {
            heartState = 2;
            return 2; //"Vfib" Erratic and high
        } else {
            heartState=1;
            return 1; //"Vtac"  Consistently high heart rate
        }
    } else {
        if ((maxBit - minBit) < 20 && !isErratic)
        {
            heartState=0;
            return 0;  //"Regular"  Consistent heart rate
        } else {
            return 4;//TODO CHECK FOR CARDIAC ARREST AGAIN
            //return -1;//"Unknown"
        }
    }
    return heartState; // Placeholder
}

bool Analyzer::checkCPR(int cBPM){
    //qDebug() << cBPM;
    //CPR BPM 100 - 120 (official requirement)
    //For testing purposes, we did 90 - 130
    // CPR BPM is considered good if it's between 90 and 130
    return (cBPM >= 90 && cBPM <= 130);
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

