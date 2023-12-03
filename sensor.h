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
    //int getCPR() const;

    //Setters
    void setPlacement(bool placement);
    void setCPRStatus(bool status);

signals:
      //void setCPR(int cBpm);

public slots:
    void receiveHeartRate(int hr);

private:
    int heartRate;
    //bool CPRStatus = false;
    bool goodPlacement = false;

    //CPR
    //int cprBPM = 0;
};

#endif // SENSOR_H
