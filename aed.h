#ifndef AED_H
#define AED_H

#include <QObject>
#include <QMainWindow>
#include "sensor.h"
#include "analyzer.h"

#include "qcustomplot.h"


#define FINAL_STATE 6
#define WAIT_MS 3000

class AED : public QObject
{
    Q_OBJECT

private:
    void updateAED();
    void batteryUpdate();
    void communicateWithUser(std::string const &);

    QMainWindow* window;
    Sensor* pSensor;
    Analyzer* pAnalyzer;
    int state = -1;
    bool isChild;
    int battery = 100;
    bool goodCPR;
    typedef std::vector<void(AED::*)()> FuncVector;
    FuncVector stateFunctions;
    bool shockPressed;

    QString feedback; // Added feedback as a member variable

    int chargeLevel = 0;
    QTimer* waitTimer;
    QTimer* updateTimer;
    QTimer *batteryTimer;

public:
    explicit AED(QObject *parent = nullptr);
    ~AED();

    bool guidePlacement(bool isChild);
    bool readyToShock();
    void setMessage(const std::string& audioMessage, const std::string& displayMessage);
    void setWaveForm(); // temp


    Sensor* getSensor();

    Analyzer* getAnalyzer();


    void incrementCharge();

    int getState() const;
    bool getIsChild() const;
    int getBattery() const;

    void setSensor(Sensor* newSensor);
    void setAnalyzer(Analyzer* newAnalyzer);
    void setState(int state);
    void setIsChild(bool child);
    void setBattery(int newBattery);

    // AED indicated stages
    void checkResponsiveness();
    void callEmergencyServices();
    void tiltHead();
    void checkAirway();
    void attachDefibPad();
    void standClear();
    void instructCPR();
    void checkAirBreathing();
    void prepareForShock();
    //extra?
 //last second setters

//        void repeatCurrentState();
//        void repeatFunction();

    float generateInterval();

    signals:
        void updateText(std::string const & s);
        void updateState(int state);

        void stateDone();
        void stageComplete();
        void updateBattery(int battery);
        void doneCharging();


        //maybe?


    public slots:
        void repeatCurrentState();
        void repeatFunction();

        void power();
        void enterNextState();
        void batteryLogic(int value);
        void setShockPressed();
};

#endif // AED_H
