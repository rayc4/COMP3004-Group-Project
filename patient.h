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
class Sensor;

class Patient : public QObject
{
    Q_OBJECT
public:
    Patient(int a, QString nm);
    ~Patient();

    int getHeartRate();
    int getAge();
    QString getName();

    void setState(int state); 
    void setCPR(bool c);

public slots:
    void updateHeartRate();
    void updateSurvivalRate();
    void reg();
    void vTac();
    void vFib();
    void cardiacArrest();
    void asystole();
    void respondToShock();
    void falseCPR();
    void receiveLeftPad(int, int);
    void receiveRightPad(int, int);

    void patientCPS();

private:
    QThread* patientThread;

    QRandomGenerator randomGen;
    int currState; //0 = reg, 1 = vtac, 2 = vfib, 3 = asys
    int age;
    bool isChild;
    QString name;
    int heartRate;
    QTimer* heartRateTimer;
    QTimer* survivalTimer;

    int survivalTime;
    int survivalChance = 100;

    QMutex heartMutex;

    //CPR Stuff
    QTime clickTime;
    int click = 0;

    bool cpr=false;
    bool cprReset = false;

signals:
    void sendHeartRate(int HR);
    void sendBPM(int bpm);
    void leftPadUpdated(int, int);
    void rightPadUpdated(int, int);
};

#endif // PATIENT_H
