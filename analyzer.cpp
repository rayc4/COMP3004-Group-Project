#include "analyzer.h"


Analyzer::Analyzer(QObject *parent) : QObject(parent), pSensor(nullptr), heartState(0), shockState(false){
}

Analyzer::~Analyzer() {

    delete pSensor;
}

void Analyzer::CollectHeart(int hbit)
{
    if (hbit != -1){
        if(heartbeats.size() >= 120)
        {
            heartbeats.pop_front();
            //qDebug() << "Popped" << heartbits.front();
        }
        heartbeats.push_back(hbit);
    }
}

int Analyzer::analyzeHeart()
{

    //add only numbers once a second
    if(heartbeats.empty() || (heartbeats.size() <=60))
    {
        return -1;
    }


    int sum = 0;
    int minBit = 60;
    int maxBit = 360;

    bool isErratic = false;

    int previousBeat = heartbeats.front();
    for(int i = 0; i < heartbeats.size(); i++)
    {
        int beat = heartbeats.at(i);
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

    double average = sum / static_cast<double>(heartbeats.size());

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

void Analyzer::checkCPR(int depth, bool isChild, QString &feedback) {
    int cprSum = 0;

    //Require a minimum of 5 values to check cpr
    int numValues = std::min(5, static_cast<int>(heartbeats.size()));

    for (int i = 0; i < numValues; ++i) {
        cprSum += heartbeats[heartbeats.size() - 1 - i];
    }

    double average = cprSum / static_cast<double>(numValues);
    feedbackString = "";

    //Whilst typical cpr is between 100-120 bpm, to make it easy we'll do 90-130
    bool isGoodCPR = (average >= 90) && (average <= 130);
    if (isGoodCPR) {
        feedbackString.append("Good CPR speed. ");
    } else {
        feedbackString.append("Poor CPR speed. ");
        if (average < 90)
            feedbackString.append("Pump faster. Aim for at least 100 BPM. ");
        else
            feedbackString.append("Slow down! Aim for at most 120BPM. ");

    }

    int lowerBound = isChild ? 45 : 50;
    int upperBound = isChild ? 55 : 60;
    int depthOffset = (depth < lowerBound) ? (depth - 45) : ((depth > upperBound) ? (depth - 55) : -1);
    bool isGoodDepth = (depth >= lowerBound) && (depth <= upperBound);

    if (isGoodDepth) {
        feedbackString.append("Good depth.");
    } else {
        feedbackString.append("Poor depth.");
        feedbackString.append((depthOffset > 0) ? QString("Lower hands by ~%1mm.").arg(depthOffset)
                                                 : QString("Raise hands by ~%1mm.").arg(qAbs(depthOffset)));
    }

    feedback = feedbackString;
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
    if (pSensor != newSensor) {
        delete pSensor;
        pSensor = newSensor;
    }
}

void Analyzer::setHeartState(int state) {
    heartState = state;
}

void Analyzer::setShockState(bool state) {
    shockState = state;
}

int Analyzer::callDepth(){
    return (pSensor->getDepth());
}
