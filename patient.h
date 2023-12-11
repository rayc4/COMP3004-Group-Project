#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMutex>

#include <QWidget>
#include <QDialog>

#include <QThread>
#include "enums.h"


class Sensor;

class Patient : public QObject
{
    Q_OBJECT
public:
    Patient(int a, QString nm);
    ~Patient();

    int getHeartRate();
    int getAge();
    bool getChild();
    QString getName();
    int getSurvival();
    HeartState getState();

    void setState(HeartState state);
    void setCPR(bool c);
    void addBreath();

    void setAge(int a);
    void setName(QString n);

    void autoCPR(); //Only a test function

public slots:
    void updateHeartRate();
    void updateSurvivalRate();
    void reg();
    void vTac();
    void vFib();
    void pulselessEA();
    void asystole();
    void respondToShock();
    void falseCPR();
    void receiveLeftPad(int, int);
    void receiveRightPad(int, int);
    void receiveDepth(int);
    void patientCPS();

private:
    QThread* patientThread;

    QRandomGenerator randomGen;


    HeartState currentState;


    //int currState; //0 = reg, 1 = vtac, 2 = vfib, 3 = asys, 4 = carr
    int age;
    bool isChild;
    QString name;
    int heartRate;
    QTimer* heartRateTimer;
    QTimer* survivalTimer;

    int survivalTime;
    int baseSurvivalChance = 100;
    int survivalBonus = 0;

    QMutex heartMutex;

    //CPR Stuff
    QTime clickTime;
    int click = 0;
    int cprCount = 0;
    int breathCount = 0;

    bool cpr=false;
    void backToLife();

    //Is set to true when at least once cpr+shock is done
    bool oneCPR = false;

signals:
    void sendHeartRate(int HR);
    void sendBPM(int bpm);
    void leftPadUpdated(int, int);
    void rightPadUpdated(int, int);
    void sendDepth (int);

};

#endif // PATIENT_H
