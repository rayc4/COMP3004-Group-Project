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

public slots:
    void receiveHeartRate(int hr);
    void setPlacement(bool placement);
    void setCPRStatus(bool status);


private:
    int heartRate;
    bool CPRStatus = false;
    bool goodPlacement = false;

};

#endif // SENSOR_H
