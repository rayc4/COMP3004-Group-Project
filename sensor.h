#ifndef SENSOR_H
#define SENSOR_H
//#include "Analyzer.h"

#include "patient.h"

class Sensor
{
public:
    Sensor(Patient& p);

    //Getters
    int getHeartRate();
    bool getCPRstatus() const;
    bool getgoodPlacement() const;

public slots:
    void setPlacement();


private:
    int heartRate;
    bool CPRstatus = false;
    bool goodPlacement = false;

    Patient& patientRef;
};

#endif // SENSOR_H
