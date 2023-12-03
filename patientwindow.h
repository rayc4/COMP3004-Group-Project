#ifndef PATIENTWINDOW_H
#define PATIENTWINDOW_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDebug>

#include "patient.h"
#include "aed.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PatientWindow; }
QT_END_NAMESPACE


class PatientWindow : public QDialog
{
    Q_OBJECT

public:
    PatientWindow(QWidget *parent = nullptr, Patient *p = nullptr, AED *a = nullptr);
    ~PatientWindow();

private slots:
    void on_vFibButton_pressed();
    void on_pushButton_clicked();

private:
    Ui::PatientWindow *ui;
    Patient *patient;
    AED *aed;

};

#endif // PATIENTWINDOW_H
