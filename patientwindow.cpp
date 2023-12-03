#include "ui_patientwindow.h"
#include "patientwindow.h"


PatientWindow::PatientWindow(QWidget *parent, Patient *p, AED *a)
    :QDialog(parent)
    , ui(new Ui::PatientWindow)
      , patient(p), aed(a)
{
    ui->setupUi(this);

    //This is temporary, the object might not be created here.
    //patient = new Patient(false, "Bob");

}

PatientWindow::~PatientWindow(){
    delete ui;
    //delete patient;
}


//This is just a test function
void PatientWindow::on_vFibButton_pressed()
{
    patient->setState(2);
    //qDebug() << "Current heartrate is " << patient->getHeartRate();
}


void PatientWindow::on_pushButton_clicked(){
    //COME BACK TO THIS. We will have to create a connection from sendBPM to setCPR in Sensor class
    // FROM there, we will want to create another signal to the AED Class
//    connect(patient, &Patient::sendBPM, aed, &AED::determineCPRStatus);
    patient->patientCPS();
}

