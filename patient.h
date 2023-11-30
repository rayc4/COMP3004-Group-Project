#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include <QThread>

#define NUM_HEARTRATES 60

class patient : public QObject
{
    Q_OBJECT
public:
    patient(bool iC, QString nm);

    QList<int>& getHeartRate();
    void setReg();

public slots:
    void updateHeartRate();
    void reg();
    void vTac();
    void vFib();
    void asystole();

private:

    bool isChild;
    QString name;
    QList<int> heartRate;
    QTimer* heartRateTimer;




};

#endif // PATIENT_H
