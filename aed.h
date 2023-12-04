#ifndef AED_H
#define AED_H

#include <QObject>
#include <QMainWindow>
#include "sensor.h"
#include "Analyzer.h"

class AED : public QObject
{
    Q_OBJECT

private:
    QMainWindow* window;
    Sensor* sensor;
    Analyzer* analyzer;
    int currentState;
    bool isChild;
    int battery;
    bool goodCPR;
    bool poweredOn = false;

public:
    explicit AED(QObject *parent = nullptr);
    ~AED();

    void updateAED();
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

 //last second setters
    void


    signals:
        //out
        void updatedStatus(QString status);
        void updateText(std::string s);

    public slots:
        //in
        void receiveSensorData(int data);
        void determineCPRStatus();
        void power();

};

#endif // AED_H
