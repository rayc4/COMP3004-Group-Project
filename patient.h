#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMutex>

#include <QThread>


class patient : public QObject
{
    Q_OBJECT
public:
    patient(bool iC, QString nm);

    int getHeartRate();
    void setState(int state);

public slots:
    void updateHeartRate();
    void reg();
    void vTac();
    void vFib();
    void asystole();

private:

    int currState; //0 = reg, 1 = vtac, 2 = vfib, 3 = asys
    bool isChild;
    QString name;
    int heartRate;
    QTimer* heartRateTimer;

    QMutex heartMutex;



};

#endif // PATIENT_H
