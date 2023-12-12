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

    // Power button
    connect(ui->powerPB, SIGNAL(clicked()), pAED, SLOT(power()));
    connect(pAED, SIGNAL(updateText(std::string const &)), this, SLOT(updateText(std::string const &)));

    // Shock button
    connect(ui->shockPB, SIGNAL(clicked()), pAED, SLOT(setShockPressed()));

    // display updates
    connect(pAED, SIGNAL(updateText(std::string)), this, SLOT(updateText(std::string)));
    connect(pAED, SIGNAL(updateState(int)), this, SLOT(updateState(int)));
    connect(ui->batterySlider, SIGNAL(sliderMoved(int)), pAED, SLOT(batteryLogic(int)));
    connect(pAED, SIGNAL(updateBattery(int)), this, SLOT(updateBatteryStatus(int)));

    stateRBs = findChildren<QRadioButton*>();
}

void MainWindow::setPatientWindow(PatientWindow* pw){
    patientWindow = pw;
}

Patient* MainWindow::getpatient()
{
    if(pPatient) return pPatient;
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
    if(pAED) return pAED;
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
    int indicatorNum = state == -1 ? 0 : state;
    QString rbName = QString("radioButton") + QString::number(indicatorNum);
    QRadioButton* rb = findChild<QRadioButton*>(rbName);
    rb->setChecked(true);
}

void MainWindow::updateBatteryStatus(int value){
    ui->batteryStatus->setValue(value);
}


