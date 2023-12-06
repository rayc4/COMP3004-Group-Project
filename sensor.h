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
    bool getgoodPlacement() const;
    int getDepth() const;

    //Setters
//    void setPlacement(bool placement);
    void setDepth(int depth);

    //last second setters
    void setPatient(Patient* tempPatient);

signals:
      //void setCPR(int cBpm);

public slots:
    void receiveHeartRate(int hr);
    void setPlacement(int r,int c);

private:
    int heartRate;
    int cprDepth;
    bool goodPlacement = false;
    Patient* pPatient;

};

#endif // SENSOR_H
