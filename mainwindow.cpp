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

    pAED = new AED(this);

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

    // THIS NEEDS TO BE IN THE SENSOR CLASS
    // connect(ui->cprPB, SIGNAL(clicked()), aed, SLOT(determineCPRStatus()));

    // Power button
    connect(ui->powerPB, SIGNAL(clicked()), pAED, SLOT(power()));
    connect(pAED, SIGNAL(updateText(std::string)), this, SLOT(updateText(std::string)));

    connect(pAED, SIGNAL(updateState(int)), this, SLOT(updateState(int)));

    stateRBs = findChildren<QRadioButton*>();
}

void MainWindow::setPatientWindow(PatientWindow* pw){
    patientWindow = pw;
}

Patient* MainWindow::getpatient()
{
    if(pPatient)
    {
        return pPatient;
    }
    return nullptr;
}

void MainWindow::generateNewPatient()
{
    //creates random data for a new patient and assigns it to the pointer in class
    Patient *tempP = new Patient(69, "Bob");
    //delete patientRef;
    pPatient = tempP;
}

void MainWindow::communicateNewPatient()
{
    //calls the setter for all the functions that need to get the updated patient


    //TODO: Call the Connected function for setting patient to the SENSOR here
    //TODO: Pass the new Patient to patient window here

    pAED->getSensor()->setPatient(pPatient);
    pPatient->setSensor(pAED->getSensor());
    patientWindow->setPatient(pPatient);

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
    if(pAED)
    {
        return pAED;
    }
    return nullptr;
}

void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << aedRef->getSensor()->getHeartRate();
    ui->heartRateLCD->display(pAED->getSensor()->getHeartRate());
}

void MainWindow::updateText(std::string s){
    QString qs = QString::fromStdString(s);
    ui->textBrowser->setText(qs);
}

void MainWindow::updateState(int state){
    QString rbName = QString("radioButton") + QString::number(state);
    QRadioButton* rb = findChild<QRadioButton*>(rbName);
    rb->setChecked(true);
}


