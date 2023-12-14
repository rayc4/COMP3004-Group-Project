#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include "patient.h"
#include "aed.h"
#include "patientwindow.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setPatientWindow(PatientWindow* pw);

    AED* getAed();
    //last second setters and getters
    Patient* getpatient();


    void generateNewPatient(); //creates random data for a new patient and assigns it to the pointer in class
    void communicateNewPatient(); //calls the setter for all the functions that need to get the updated patient

    void initPatient(); //deals with all the calls for creating and communicating the new patient


private slots:
    void updateText(std::string s);
    void updateState(int state);
    void updateBatteryStatus (int value);
    void enableShockPB();
    void updateGraph();

private:
    void updateGUI();

    Ui::MainWindow *ui;
    Patient *pPatient;
    AED *pAED;
    PatientWindow* patientWindow;
    QList<QRadioButton*> stateRBs;

    QCustomPlot *customPlot;
    double time = 0;
    int counter = 0;
    int currentInterval = 0;
    bool fibrillation = false;
    double bpm = 75;

    int bpmOffset = 0;

    HeartState currentState;
    QRandomGenerator randomGen;
    QRandomGenerator randomFluctuation;

    QTimer* heartRateTimer;
};
#endif // MAINWINDOW_H
