#include "mainwindow.h"
#include "patientwindow.h"
#include "patient.h"
#include "aed.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Patient *patient = new Patient(48, "Bob");
    AED *aed = new AED(nullptr);

    PatientWindow *patientW = new PatientWindow(nullptr, patient, aed);
    patientW->show();

    MainWindow *w = new MainWindow(nullptr, patient, aed);
    w->show();

    //This does not work at the moment:
    //delete patient;
    //delete patientW;
    //delete aed;

    return a.exec();
}
