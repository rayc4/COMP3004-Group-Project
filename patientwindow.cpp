#include "ui_patientwindow.h"
#include "patientwindow.h"


PatientWindow::PatientWindow(QWidget *parent, Patient *p, AED *a)
    :QDialog(parent)
    , ui(new Ui::PatientWindow)
      , patient(p), aed(a)
{
    ui->setupUi(this);



//    QTimer* guiTimer = new QTimer();
//    connect(guiTimer, &QTimer::timeout, [=]() {
//        updateGUI();
//    });
//    guiTimer->start(300);

   ui->nameLabel->setText(QString("Patient Name: %1").arg(patient->getName()));
   ui->ageLabel->setText(QString("Age: %1").arg(patient->getAge()));

}

PatientWindow::~PatientWindow(){
    delete ui;
    //delete patient;
}


//void PatientWindow::updateGUI(){

//}

void PatientWindow::on_pushButton_clicked(){
    patient->patientCPS();
    aed->determineCPRStatus();
}


void PatientWindow::on_killButton_clicked(bool checked)
{
    if (checked){
        patient->setState(3);
        ui->vFibButton->setDisabled(1);
        ui->vTacButton->setDisabled(1);
    }
    else{
        patient->setState(0);
        ui->vFibButton->setDisabled(0);
        ui->vTacButton->setDisabled(0);
    }


}


void PatientWindow::on_vFibButton_clicked(bool checked)
{
    if (checked){
        patient->setState(2);
        ui->killButton->setDisabled(1);
        ui->vTacButton->setDisabled(1);
    }
    else{
        patient->setState(0);
        ui->killButton->setDisabled(0);
        ui->vTacButton->setDisabled(0);
    }
}


void PatientWindow::on_vTacButton_clicked(bool checked)
{
    if (checked){
        patient->setState(1);
        ui->vFibButton->setDisabled(1);
        ui->killButton->setDisabled(1);
    }
    else{
        patient->setState(0);
        ui->vFibButton->setDisabled(0);
        ui->killButton->setDisabled(0);
    }
}


void PatientWindow::on_verticalSlider_valueChanged(int value)
{
    qDebug() << "Current value is " << value;
}


void PatientWindow::on_leftPadTable_cellPressed(int row, int column)
{
    //Note, the current row/column for this is the position of the pad!!

    ui->leftPadTable->clear();
    ui->leftPadTable->clearSelection();

    int startRow = row - 2;
    int startColumn = column - 2;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            int tempRow = startRow + i;
            int tempColumn = startColumn + j;

            if (tempRow >= 0 && tempRow < ui->leftPadTable->rowCount() &&
                tempColumn >= 0 && tempColumn < ui->leftPadTable->columnCount()) {
                QTableWidgetItem *tempItem = new QTableWidgetItem("");
                tempItem->setBackground(QBrush(Qt::blue));
                ui->leftPadTable->setItem(tempRow, tempColumn, tempItem);
            }
        }
    }
}




void PatientWindow::on_rightPadTable_cellPressed(int row, int column)
{
    //Note, the current row/column for this is the position of the pad!!

    ui->rightPadTable->clear();
    ui->rightPadTable->clearSelection();

    int startRow = row - 2;
    int startColumn = column - 2;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            int tempRow = startRow + i;
            int tempColumn = startColumn + j;

            if (tempRow >= 0 && tempRow < ui->rightPadTable->rowCount() &&
                tempColumn >= 0 && tempColumn < ui->rightPadTable->columnCount()) {
                QTableWidgetItem *tempItem = new QTableWidgetItem("");
                tempItem->setBackground(QBrush(Qt::blue));
                ui->rightPadTable->setItem(tempRow, tempColumn, tempItem);
            }
        }
    }
}

