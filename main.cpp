#include "mainwindow.h"
#include "patientwindow.h"
#include "patient.h"
#include "aed.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //mainwindow creates AED
    //mainwidnow has AED getter

    //patientwindow is created, pass mainwindow getter through connt argument

    MainWindow *w = new MainWindow(nullptr);
    w->show();

    PatientWindow *patientW = new PatientWindow(w,,w->getAed());

    patientW->show();


    //This does not work at the moment:
    //delete patient;
    //delete patientW;
    //delete aed;

    return a.exec();
}
