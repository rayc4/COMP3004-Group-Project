#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent, Patient *patient, AED *aed)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      patientRef(patient), aedRef(aed)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton1_clicked()
{
    std::cout << "Button Pushed" << std::endl;
}


void MainWindow::on_pushButton_clicked()
{
    connect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);
}


void MainWindow::on_pushButton_2_clicked()
{
    disconnect(patientRef, &Patient::sendHeartRate, aedRef->getSensor(), &Sensor::receiveHeartRate);
}

