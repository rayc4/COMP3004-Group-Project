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
    PatientWindow(QWidget *parent = nullptr);
    ~PatientWindow();


    Patient* getPatient();

    //last second setters

    void setPatient(Patient* aNewPatient);


private slots:
    void on_killButton_clicked(bool checked);

    void on_vFibButton_clicked(bool checked);

    void on_vTacButton_clicked(bool checked);

    void on_verticalSlider_valueChanged(int value);


    void on_leftPadTable_cellPressed(int row, int column);

    void on_rightPadTable_cellPressed(int row, int column);


    void on_cardiacButton_clicked(bool checked);

    void on_updateButton_pressed();

    void on_updateButton_released();

    void on_updateButton_clicked();

    void on_breathButton_clicked();

    void on_shockTestButton_clicked();

    void on_autoCPRButton_clicked();

    void on_breathButton_pressed();

    void on_breathButton_released();

    void on_airwayButton_clicked(bool checked);

    void on_tiltButton_clicked(bool checked);

signals:
    void declarePosition(bool placement);

private:
    Ui::PatientWindow *ui;
    Patient *pPatient;
    AED *pAED;

    void updateGUI();
    void resetAllButtons();

};

#endif // PATIENTWINDOW_H
