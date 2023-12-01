#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include "sensor.h"

class Analyzer : public QObject {
    Q_OBJECT


private:
    Sensor* sensor;
    int heartState;
    bool shockState;



public:
   Analyzer(QObject *parent = nullptr);
    ~Analyzer();

 
    int analyzeHeart();
    bool CPRFeedback(const std::string& feedback);
    void setShock(bool shock);
    bool checkPlacement();

 
    Sensor* getSensor() const;
    int getHeartState() const;
    bool getShockState() const;

 
    void setSensor(Sensor* newSensor);
    void setHeartState(int state);
    void setShockState(bool state);

//signals:
    //out

//public slots:
    //in

};

#endif // ANALYZER_H
