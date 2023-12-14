#include "ui_patientwindow.h"
#include "patientwindow.h"


PatientWindow::PatientWindow(QWidget *parent)
    :QDialog(parent), ui(new Ui::PatientWindow)
{
    ui->setupUi(this);

    QTimer* guiTimer = new QTimer();
    connect(guiTimer, &QTimer::timeout, [=]() {
        updateGUI();
    });
    guiTimer->start(300);

    ui->shockTestButton->setVisible(false);
    ui->cardiacButton->setVisible(false);
    ui->killButton->setVisible(false);
    ui->stateLabel->setVisible(false);



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
    connect(pPatient, &Patient::sendBreath, this, &PatientWindow::receiveBreath);
}



PatientWindow::~PatientWindow(){
    delete ui;
    //delete patient;
}

void PatientWindow::updateGUI()
{
    //main visual updates should happen here

    //Update survival chance:
    QString survivalString = QString("<html><head/><body>"
                        "<p><span style=\"font-size:9pt; font-weight:600;\">SURVIVAL<br/>CHANCE<br/></span>"
                        "<span style=\"font-size:16pt; font-weight:600;\">%1%</span></p>"
                        "</body></html>").arg(pPatient->getSurvival());
    ui->survivalLabel->setText(survivalString);





    //Update current state on GUI
    QString stateString = "<html><head/><body>"
                         "<p><span style=\"font-weight:600;\">CURRENT<br/>STATE<br/></span>"
                         "<span style=\"font-size:20pt; font-weight:600;\">%1</span></p>"
                         "</body></html>";
    switch (pPatient->getState()) {
        case REG:
            ui->stateLabel->setText(stateString.arg("REG"));
            resetAllButtons();
            break;
        case VTAC:
            ui->stateLabel->setText(stateString.arg("VTAC"));
            break;
        case VFIB:
            ui->stateLabel->setText(stateString.arg("VFIB"));
            break;
        case ASYS:
            ui->stateLabel->setText(stateString.arg("DEAD"));
            break;
        case PEA:
            ui->stateLabel->setText(stateString.arg("PEA"));
            break;
        default:    //Not sure how it would get to this state
            ui->stateLabel->setText(stateString.arg("?"));

    }






}

void PatientWindow::resetAllButtons(){
    ui->killButton->setChecked(false);
    ui->killButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
    ui->vTacButton->setChecked(false);
    ui->vTacButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
    ui->vFibButton->setChecked(false);
    ui->vFibButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
    ui->cardiacButton->setChecked(false);
    ui->cardiacButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
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
        if (pPatient->getState() == REG){ // Only allowed if in regular state
            pPatient->setState(ASYS);
            ui->killButton->setStyleSheet("border: 4px solid rgb(0, 0, 0);"
                                             "color: rgb(0,0,0);");
        }
        else
            ui->killButton->setChecked(false);
    }
    else{
        pPatient->setState(REG);
        ui->killButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");
    }
    updateGUI();

}

void PatientWindow::on_cardiacButton_clicked(bool checked)
{
    if (checked){
        if (pPatient->getState() == REG){ // Only allowed if in regular state
            ui->cardiacButton->setStyleSheet("border: 4px solid rgb(237, 51, 59);"
                                             "color: rgb(237, 51, 59);");
            pPatient->setState(PEA);
        }
        else
            ui->cardiacButton->setChecked(false);
    }
    else{
        ui->cardiacButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");
        pPatient->setState(REG);
    }
    updateGUI();
}

void PatientWindow::on_vFibButton_clicked(bool checked)
{
    if (checked){
        if (pPatient->getState() == REG){ // Only allowed if in regular state
            pPatient->setState(VFIB);
            ui->vFibButton->setStyleSheet("border: 4px solid rgb(53, 132, 228);"
                                             "color: rgb(53, 132, 228);");
        }
        else
            ui->vFibButton->setChecked(false);
    }
    else{
        ui->vFibButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");
        pPatient->setState(REG);
    }
    updateGUI();
}


void PatientWindow::on_vTacButton_clicked(bool checked)
{
    if (checked){
        if (pPatient->getState() == REG){ // Only allowed if in regular state
            pPatient->setState(VTAC);
            ui->vTacButton->setStyleSheet("border: 4px solid rgb(129, 61, 156);"
                                             "color: rgb(129, 61, 156);");
        }
        else
            ui->vTacButton->setChecked(false);
    }
    else{
        ui->vTacButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");
        pPatient->setState(REG);
    }
    updateGUI();
}


//void PatientWindow::on_verticalSlider_valueChanged(int value)
//{
//    //circular call
//    //aed->getSensor()->setDepth(value);
//    //qDebug() << "Current value is " << value;

//    //TODO: delete this
//}


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





void PatientWindow::on_updateButton_pressed()
{
    ui->updateButton->setStyleSheet("border: 4px solid rgb(200, 200, 200);"
                                     "color: rgb(200,200,200);");
}


void PatientWindow::on_updateButton_released()
{
    ui->updateButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
}


void PatientWindow::on_updateButton_clicked()
{
    pPatient->setAge(ui->ageLine->text().toInt());
    pPatient->setName(ui->nameLine->text());
    updateGUI();
}


void PatientWindow::on_breathButton_clicked()
{
    pPatient->addBreath();
    updateGUI();
}


void PatientWindow::on_shockTestButton_clicked()
{
    pPatient->respondToShock();
}


void PatientWindow::on_autoCPRButton_clicked()
{
    pPatient->autoCPR();
}


void PatientWindow::on_breathButton_pressed()
{
    ui->breathButton->setStyleSheet("border: 4px solid rgb(200, 200, 200);"
                                     "color: rgb(200,200,200);");
}


void PatientWindow::on_breathButton_released()
{
    ui->breathButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
}


void PatientWindow::on_airwayButton_clicked(bool checked)
{
    if (checked)
        ui->airwayButton->setStyleSheet("border: 4px solid rgb(200, 200, 200);"
                                     "color: rgb(200,200,200);");
    else
        ui->airwayButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");



    if (ui->tiltButton->isChecked() && checked){
        ui->breathButton->setEnabled(true);
        ui->breathButton->setText("BREATH");
    }
    else{
        ui->breathButton->setEnabled(false);
        ui->breathButton->setText("");
    }

}


void PatientWindow::on_tiltButton_clicked(bool checked)
{
    if (checked)
        ui->tiltButton->setStyleSheet("border: 4px solid rgb(200, 200, 200);"
                                     "color: rgb(200,200,200);");
    else
        ui->tiltButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");


    if (ui->airwayButton->isChecked() && checked){
        ui->breathButton->setEnabled(true);
        ui->breathButton->setText("BREATH");
    }
    else{
        ui->breathButton->setEnabled(false);
        ui->breathButton->setText("");
    }
}

void PatientWindow::receiveBreath(){
    if (ui->breathButton->isEnabled()){
        ui->breathButton->setStyleSheet("border: 4px solid rgb(0,150,200);"
                                        "color: rgb(0,150,200);");
        QTimer::singleShot(1000, this, &PatientWindow::breathOff);
    }
}

void PatientWindow::breathOff(){
    ui->breathButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                     "color: rgb(0,0,0);");
}

void PatientWindow::on_disableSurvivalButton_clicked(bool checked)
{
    if (checked)
        ui->disableSurvivalButton->setStyleSheet("border: 4px solid rgb(255, 50, 50);"
                                     "color: rgb(255, 50, 50);");
    else
        ui->disableSurvivalButton->setStyleSheet("border: 4px solid rgb(154, 153, 150);"
                                         "color: rgb(0,0,0);");

    pPatient->setSurvivalTimer(checked);
}


void PatientWindow::on_forceCombo_currentIndexChanged(int index)
{
    pPatient->setSequence(index);
    if (index == 1){
        ui->vFibButton->setStyleSheet("border: 4px solid rgb(129, 61, 156);"
                                         "color: rgb(129, 61, 156);");
        ui->vFibButton->setChecked(true);
    }
    else if (index == 2){
        ui->vTacButton->setStyleSheet("border: 4px solid rgb(129, 61, 156);"
                                         "color: rgb(129, 61, 156);");
        ui->vTacButton->setChecked(true);
    }

}


void PatientWindow::on_checkBox_clicked(bool checked)
{
    if (checked){
        ui->shockTestButton->setVisible(false);
        ui->cardiacButton->setVisible(false);
        ui->killButton->setVisible(false);
        ui->stateLabel->setVisible(false);
    }
    else{
        ui->shockTestButton->setVisible(true);
        ui->cardiacButton->setVisible(true);
        ui->killButton->setVisible(true);
        ui->stateLabel->setVisible(true);
        ui->forceLabel->setVisible(true);

    }
}

