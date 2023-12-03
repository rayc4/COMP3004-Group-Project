#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent, Patient *patient, AED *aed)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      patientRef(patient), aedRef(aed)
{
    ui->setupUi(this);


    QTimer* guiTimer = new QTimer();
    connect(guiTimer, &QTimer::timeout, [=]() {
        updateGUI();
    });
    guiTimer->start(300);

    connect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << aedRef->getSensor()->getHeartRate();
    ui->heartRateLCD->display(aedRef->getSensor()->getHeartRate());
}

void MainWindow::on_pushButton1_clicked()
{
    std::cout << "Button Pushed" << std::endl;
}


