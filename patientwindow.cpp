#include "ui_patientwindow.h"
#include "patientwindow.h"


PatientWindow::PatientWindow(QWidget *parent)
    :QDialog(parent), ui(new Ui::PatientWindow)
{
    ui->setupUi(this);

//    QTimer* guiTimer = new QTimer();
//    connect(guiTimer, &QTimer::timeout, [=]() {
//        updateGUI();
//    });
//    guiTimer->start(300);



//    void PatientWindow::on_pushButton_clicked(){
//        patient->patientCPS();
//        aed->determineCPRStatus();
//    }

}

void PatientWindow::setPatient(Patient* tempPatient)
{
    if(!tempPatient)
    {
        return;
    }
    pPatient = tempPatient;
    ui->nameLine->setText(pPatient->getName());
    ui->ageLine->setText(QString::number(pPatient->getAge()));

    connect(ui->cprPB, SIGNAL(clicked()), pPatient, SLOT(patientCPS()));
    connect(ui->leftPadTable, SIGNAL(cellPressed(int,int)), pPatient, SLOT(receiveLeftPad(int,int)));
    connect(ui->rightPadTable, SIGNAL(cellPressed(int,int)), pPatient, SLOT(receiveRightPad(int,int)));
    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), pPatient, SLOT(receiveDepth(int)));
}



PatientWindow::~PatientWindow(){
    delete ui;
    //delete patient;
}


void PatientWindow::updateGUI()
{
//main visual updates should happen here
}

Patient* PatientWindow::getPatient()
{
    if(pPatient)
    {
    return pPatient;
    }
    return nullptr;
}


void PatientWindow::on_killButton_clicked(bool checked)
{
    if (checked){
        pPatient->setState(3);
        ui->vFibButton->setDisabled(1);
        ui->vTacButton->setDisabled(1);
        ui->cardiacButton->setDisabled(1);
    }
    else{
        pPatient->setState(0);
        ui->vFibButton->setDisabled(0);
        ui->vTacButton->setDisabled(0);
        ui->cardiacButton->setDisabled(0);
    }


}

void PatientWindow::on_cardiacButton_clicked(bool checked)
{
    if (checked){
        pPatient->setState(4);
        ui->vFibButton->setDisabled(1);
        ui->vTacButton->setDisabled(1);
        ui->killButton->setDisabled(1);
    }
    else{
        pPatient->setState(0);
        ui->vFibButton->setDisabled(0);
        ui->vTacButton->setDisabled(0);
        ui->killButton->setDisabled(0);
    }
}

void PatientWindow::on_vFibButton_clicked(bool checked)
{
    if (checked){
        pPatient->setState(2);
        ui->killButton->setDisabled(1);
        ui->vTacButton->setDisabled(1);
        ui->cardiacButton->setDisabled(1);
    }
    else{
        pPatient->setState(0);
        ui->killButton->setDisabled(0);
        ui->vTacButton->setDisabled(0);
        ui->cardiacButton->setDisabled(0);
    }
}


void PatientWindow::on_vTacButton_clicked(bool checked)
{
    if (checked){
        pPatient->setState(1);
        ui->vFibButton->setDisabled(1);
        ui->killButton->setDisabled(1);
        ui->cardiacButton->setDisabled(1);
    }
    else{
        pPatient->setState(0);
        ui->vFibButton->setDisabled(0);
        ui->killButton->setDisabled(0);
        ui->cardiacButton->setDisabled(0);
    }
}


void PatientWindow::on_verticalSlider_valueChanged(int value)
{
    //circular call
    //aed->getSensor()->setDepth(value);
    //qDebug() << "Current value is " << value;
}


void PatientWindow::on_leftPadTable_cellPressed(int row, int column)
{
    //Note, the current row/column for this is the position of the pad!!

    //Reduce size of pads if child...
    int size = 0;
    if (pPatient->getChild())
        size = -1;

    ui->leftPadTable->clear();
    ui->leftPadTable->clearSelection();

    int startRow = row - 2 - size;
    int startColumn = column - 2 - size;

    for (int i = 0; i < 4+size; i++) {
        for (int j = 0; j < 5+size; j++) {
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

    //Reduce size of pads if child...
    int size = 0;
    if (pPatient->getChild())
        size = -1;


    ui->rightPadTable->clear();
    ui->rightPadTable->clearSelection();

    int startRow = row - 2 - size;
    int startColumn = column - 2 - size;

    for (int i = 0; i < 4+size; i++) {
        for (int j = 0; j < 5+size; j++) {
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


void PatientWindow::on_pushButton_clicked()
{
    pPatient->setAge(ui->ageLine->text().toInt());
    pPatient->setName(ui->nameLine->text());

}

