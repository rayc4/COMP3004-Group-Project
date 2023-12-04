#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //treat this as your casual and traditional main
    //mainwindow OWNS patient
    ui->setupUi(this);

    aedRef = new AED(this);

    QTimer* guiTimer = new QTimer();
    connect(guiTimer, &QTimer::timeout, [=]() {
        updateGUI();
    });
    guiTimer->start(300);

    //connect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);
    //replace with observer and logical steps from
        //sensor
        //analyzer

    //connect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);

    // Power button
    connect(ui->powerPB, SIGNAL(clicked()), aedRef, SLOT(power()));
    connect(aedRef, SIGNAL(updateText(std::string)), this, SLOT(updateText(std::string)));

    // THIS NEEDS TO BE IN THE SENSOR CLASS
    // connect(ui->cprPB, SIGNAL(clicked()), aed, SLOT(determineCPRStatus()));

}

void MainWindow::setPatientWindow(PatientWindow* pw){
    patientWindow = pw;
}

Patient* MainWindow::getpatient()
{
    if(patientRef)
    {
        return patientRef;
    }
    return nullptr;
}

void MainWindow::generateNewPatient()
{
    //creates random data for a new patient and assigns it to the pointer in class
    Patient *tempP = new Patient(69, "Bob");
    patientRef = tempP;
}

void MainWindow::communicateNewPatient()
{
    //calls the setter for all the functions that need to get the updated patient
    aedRef->getSensor()->setPatient(patientRef);
    patientWindow->setPatient(patientRef);
}


void MainWindow::initPatient()
{
    //deals with all the calls for creating and communicating the new patient
    generateNewPatient();
    communicateNewPatient();
}


MainWindow::~MainWindow()
{
    delete ui;
}


AED* MainWindow::getAed()
{
    if(aedRef)
    {
        return aedRef;
    }
    return nullptr;
}

void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << aedRef->getSensor()->getHeartRate();
    ui->heartRateLCD->display(aedRef->getSensor()->getHeartRate());
}

void MainWindow::updateText(std::string s){
    QString qs = QString::fromStdString(s);
    ui->textBrowser->setText(qs);
}


