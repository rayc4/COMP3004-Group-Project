#ifndef SENSOR_H
#define SENSOR_H
//#include "Analyzer.h"
#include <QObject>

#include "patient.h"

class Sensor : public QObject
{
    Q_OBJECT

public:
    Sensor();

    //Getters
    int getHeartRate();
    bool getCPRstatus() const;
    bool getgoodPlacement() const;
    int getDepth() const;

    //Setters
    void setPlacement(bool placement);
    void setDepth(int depth);


signals:
      //void setCPR(int cBpm);

public slots:
    void receiveHeartRate(int hr);

private:
    int heartRate;
    int cprDepth;
    bool goodPlacement = false;
};

#endif // SENSOR_H
