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

    void patientCPS();





public slots:
    void updateHeartRate();
    void updateSurvivalRate();
    void reg();
    void vTac();
    void vFib();
    void asystole();
    void respondToShock();

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
    int survivalAddition = 0;

    QMutex heartMutex;

    //CPR Stuff
    QTime clickTime;
    int click = 0;

    bool cpr=false;



signals:

    void sendHeartRate(int HR);
    void sendBPM(int bpm);


};

#endif // PATIENT_H
