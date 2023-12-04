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
    PatientWindow(QWidget *parent = nullptr, AED *a = nullptr);
    ~PatientWindow();


    Patient* getPatient();

private slots:
    void on_pushButton_clicked();

    void on_killButton_clicked(bool checked);

    void on_vFibButton_clicked(bool checked);

    void on_vTacButton_clicked(bool checked);

    void on_verticalSlider_valueChanged(int value);


    void on_leftPadTable_cellPressed(int row, int column);

    void on_rightPadTable_cellPressed(int row, int column);


    void on_cardiacButton_clicked(bool checked);

private:
    Ui::PatientWindow *ui;
    Patient *patient;
    AED *aed;

    void updateGUI();

};

#endif // PATIENTWINDOW_H
