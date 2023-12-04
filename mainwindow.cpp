#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    aedRef = new AED(this);

    QTimer* guiTimer = new QTimer();
    connect(guiTimer, &QTimer::timeout, [=]() {
        updateGUI();
    });
    guiTimer->start(300);

    //connect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);
    //replace with observer and logical steps from
        //sensor
        //analyzer

}

MainWindow::~MainWindow()
{
    delete ui;
}


AED* MainWindow::getAed()
{
    if(aedRef)
    {
        return aedRef;
    }
    return nullptr;
}

void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << aedRef->getSensor()->getHeartRate();
    ui->heartRateLCD->display(aedRef->getSensor()->getHeartRate());
}

void MainWindow::on_pushButton1_clicked()
{
    std::cout << "Button Pushed" << std::endl;
}


