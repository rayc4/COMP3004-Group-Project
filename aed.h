#ifndef AED_H
#define AED_H

#include <QObject>
#include "sensor.h"
#include "Analyzer.h"
#include "patient.h"

class AED : public QObject
{
    Q_OBJECT

private:
    Sensor* sensor;
    Analyzer* analyzer;
    int currentState;
    bool isChild;
    int battery;

public:
    explicit AED(QObject *parent = nullptr);
    ~AED();

    void updateAED();
    void powerOn();
    bool guidePlacement(bool isChild);
    bool readyToShock();
    void setMessage(const std::string& audioMessage, const std::string& displayMessage);
    void setWaveForm(); // temp


    Sensor* getSensor() const;
    Analyzer* getAnalyzer() const;
    int getCurrentState() const;
    bool getIsChild() const;
    int getBattery() const;

    void setSensor(Sensor* newSensor);
    void setAnalyzer(Analyzer* newAnalyzer);
    void setCurrentState(int state);
    void setIsChild(bool child);
    void setBattery(int newBattery);


    signals:
        //out
        void updatedStatus(QString status);

    public slots:
        //in
        void receiveSensorData(int data);

};

#endif // AED_H
