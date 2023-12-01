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
    void setPlacement(bool placement);
    void setCPRStatus(bool status);


private:
    int heartRate;
    bool CPRStatus = false;
    bool goodPlacement = false;

    Patient& patientRef;
};

#endif // SENSOR_H
