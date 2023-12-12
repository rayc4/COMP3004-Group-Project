#include "mainwindow.h"
#include "patientwindow.h"
#include "patient.h"
#include "aed.h"

#include <QApplication>
#include <QDebug>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //implemented testing code
    //this will launch multiple instances
    //to be able to test if our project will always compile correctly, and not by coincidence

    const int numberOfInstances = 10;
    // Set the number of instances here
    //set 1 if not testing
    std::vector<MainWindow*> windows;
    std::vector<PatientWindow*> patientWindows;
    int initializedWindows = 0;

    for (int i = 0; i < numberOfInstances; ++i) {
        try {
            MainWindow *w = new MainWindow(nullptr);
            windows.push_back(w);
            w->show();
            qDebug() << "MainWindow instance" << i + 1 << "initialized";

            PatientWindow *patientW = new PatientWindow(w);
            patientWindows.push_back(patientW);
            patientW->show();
            qDebug() << "PatientWindow instance" << i + 1 << "initialized";

            w->setPatientWindow(patientW);
            w->initPatient();

            initializedWindows += 2;
            // gotta use base 2
            //because we are accounting for
            //for MainWindow and PatientWindow
        } catch (const std::exception& e) {
            qDebug() << "Exception occurred while creating window instance:" << e.what();
        }
    }

    int result = a.exec();

    // Check if all windows were initialize
    if (initializedWindows == numberOfInstances * 2)
    {
        qDebug() << "All windows initialized successfully.";
    }
    else
    {
        qDebug() << "Some windows failed to initialize.";
    }

    // Clean up
    for (auto *window : windows)
    {
        delete window;
    }
    for (auto *patientWindow : patientWindows)
    {
        delete patientWindow;
    }
    //easier using for auto
    return result;
}
