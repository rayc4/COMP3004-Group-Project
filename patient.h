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
    Patient(bool iC=false, QString nm=NULL);
    ~Patient();

    int getHeartRate();
    void setState(int state);



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
    bool isChild;
    QString name;
    int heartRate;
    QTimer* heartRateTimer;
    QTimer* survivalTimer;

    int survivalTime;
    int survivalAddition = 0;

    QMutex heartMutex;


signals:
    void sendHeartRate(int HR);



};

#endif // PATIENT_H
