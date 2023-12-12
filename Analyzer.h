#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <deque>
#include "sensor.h"

#include "enums.h"


class Analyzer : public QObject {
    Q_OBJECT


private:
    Sensor* pSensor;
    int heartState;
    bool shockState;
    int cBPM;
    bool goodCPR;
    std::deque<int> heartbeats;

    QString feedbackString;


public:
   Analyzer(QObject *parent = nullptr);
    ~Analyzer();

 
    int analyzeHeart();
    void setShock(bool shock);
    bool checkPlacement();

 
    Sensor* getSensor() const;
    int getHeartState() const;
    bool getShockState() const;

 
    void setSensor(Sensor* newSensor);
    void setHeartState(int state);
    void setShockState(bool state);
    /* Regular, asystole, vtac, vfib
     * Check for placement, check for cpr (depth AND speed)
    */
    void checkCPR(int depth, bool isChild, QString &feedback);

    void CollectHeart(int hbit);
    int callDepth();


//signals:
    //out

//public slots:
    //in

};

#endif // ANALYZER_H
