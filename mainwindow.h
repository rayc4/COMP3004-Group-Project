#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "patient.h"
#include "aed.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



AED* getAed();
//last second setters and getters
Patient* getpatient();


void generateNewPatient(); //creates random data for a new patient and assigns it to the pointer in class
void communicatenewPatient(); //calls the setter for all the functions that need to get the updated patient

void createNewPatient(); //deals with all the calls for creating and communicating the new patient


private slots:
    void updateText(std::string s);

private:
    void updateGUI();

    Ui::MainWindow *ui;
    Patient *patientRef;
    AED *aedRef;

};
#endif // MAINWINDOW_H
