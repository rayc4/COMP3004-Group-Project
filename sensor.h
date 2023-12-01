#ifndef SENSOR_H
#define SENSOR_H
//#include "Analyzer.h"

class sensor
{
public:
    sensor();

    //Getters
    int getHeartRate() const;
    bool getCPRstatus() const;
    bool getgoodPlacement() const;

private:
    int heartRate;
    bool CPRstatus = false;
    bool goodPlacement = true;
};

#endif // SENSOR_H
