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
    connect(ui->powerPB, SIGNAL(clicked()), aedRef, SLOT(power()));
    connect(aedRef, SIGNAL(updateText(std::string)), this, SLOT(updateText(std::string)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << aedRef->getSensor()->getHeartRate();
    ui->heartRateLCD->display(aedRef->getSensor()->getHeartRate());
}

void MainWindow::updateText(std::string s){
    QString qs = QString::fromStdString(s);
    ui->textBrowser->setText(qs);
}


