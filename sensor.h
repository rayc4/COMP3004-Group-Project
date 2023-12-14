#ifndef SENSOR_H
#define SENSOR_H
//#include "Analyzer.h"
#include <QObject>

#include "patient.h"

class Sensor : public QObject
{
    Q_OBJECT

public:
    Sensor(Patient* aPatient = nullptr);

    //Getters
    int getHeartRate();
    bool getCPRstatus() const;
    bool getGoodPlacement();
    int getDepth();
    bool getChild();

    //last second setters
    void setPatient(Patient* tempPatient);
    void checkPads();

    void sendShock();

//signals:
      //void setCPR(int cBpm);

public slots:
    //void receiveHeartRate(int hr);
    void setLPlacement(int r,int c);
    void setRPlacement(int r,int c);
    void setDepth(int depth);

private:
    int heartRate;
    int cprDepth = 0;
    bool goodPlacement = false;
    bool leftPad = false;
    bool rightPad = false;
    bool pressureCPR = false;
    bool isChild = false;
    Patient* pPatient;

};

#endif // SENSOR_H
