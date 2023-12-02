#include "ui_patientwindow.h"
#include "patientwindow.h"


PatientWindow::PatientWindow(QWidget *parent, Patient *p)
    :QDialog(parent)
    , ui(new Ui::PatientWindow)
      , patient(p)
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

