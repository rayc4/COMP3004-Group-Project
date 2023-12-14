#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "enums.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //treat this as your casual and traditional main
    //mainwindow OWNS patient
    ui->setupUi(this);

    pAED = new AED(this);

    QTimer* guiTimer = new QTimer();
    connect(guiTimer, &QTimer::timeout, [=]() {
        updateGUI();
    });
    guiTimer->start(300);

    // Power button
    connect(ui->powerPB, SIGNAL(clicked()), pAED, SLOT(power()));
    connect(pAED, SIGNAL(updateText(std::string const &)), this, SLOT(updateText(std::string const &)));

    // Shock button
    connect(ui->shockPB, SIGNAL(clicked()), pAED, SLOT(setShockPressed()));
    connect(pAED, SIGNAL(doneCharging()), this, SLOT(enableShockPB()));

    // display updates
    connect(pAED, SIGNAL(updateText(std::string)), this, SLOT(updateText(std::string)));
    connect(pAED, SIGNAL(updateState(int)), this, SLOT(updateState(int)));
    connect(ui->batterySlider, SIGNAL(sliderMoved(int)), pAED, SLOT(batteryLogic(int)));
    connect(pAED, SIGNAL(updateBattery(int)), this, SLOT(updateBatteryStatus(int)));

    stateRBs = findChildren<QRadioButton*>();

    // Graph
    customPlot = new QCustomPlot(ui->customPlot);

    QCPGraph *graph = customPlot->addGraph();
    graph->setLineStyle(QCPGraph::lsLine);
    graph->setPen(QPen(Qt::red));
    graph->setName("ECG");




    customPlot->setMinimumSize(ui->customPlot->width(), ui->customPlot->height());
    //customPlot->xAxis->setLabel("Time (ms)");
    customPlot->xAxis->setRange(0, 2000);
    customPlot->yAxis->setRange(-50, 500);
    customPlot->xAxis->setTickLabels(false);
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis->setTickLabels(false);
    customPlot->yAxis->setTicks(false);



    currentState = REG;

    heartRateTimer = new QTimer();
    connect(heartRateTimer, &QTimer::timeout, [=]() {
        updateGraph();
    });
    currentInterval = pAED->generateInterval();
    heartRateTimer->start(currentInterval);

    randomGen.seed(QTime::currentTime().msec());
    randomFluctuation.seed(QTime::currentTime().msec());


    graph->setAdaptiveSampling(true); //This smooths out harsh zig-zags

}

void MainWindow::updateGraph() {
    HeartState heartState = REG;
    int aedState;
    heartState = pAED->getAnalyzer()->getHeartState();
    aedState = pAED->getState();

    heartRateTimer->setInterval(currentInterval);
    if (aedState >=4){
        if (heartState == REG){
            // Hardcoding datapoints for a regular heartbeat
            if (counter == 0) {
                customPlot->graph(0)->addData(time, 0);
                customPlot->graph(0)->addData(time + 3, 10 + randomFluctuation.bounded(-2, 2));
                customPlot->graph(0)->addData(time + 6, 0);
            } else if (counter == 1) {
                customPlot->graph(0)->addData(time + 10, 0);
            } else if (counter == 2) {
                customPlot->graph(0)->addData(time + 14, -8 + randomFluctuation.bounded(-2, 2));
            } else if (counter == 3) {
                ui->bpmLabel->setStyleSheet("color: red;");
                customPlot->graph(0)->addData(time + 20, 60 + randomFluctuation.bounded(-2, 2));
                ui->bpmLabel->setText(QString("♥ BPM: %1").arg(pAED->getSensor()->getHeartRate()));
            } else if (counter == 4) {
                customPlot->graph(0)->addData(time + 24, -15 + randomFluctuation.bounded(-2, 2));
            } else if (counter == 5) {
                customPlot->graph(0)->addData(time + 28, 0);
                ui->bpmLabel->setStyleSheet("color: black;");
            } else if (counter == 6) {
                customPlot->graph(0)->addData(time + 32, 0);
                customPlot->graph(0)->addData(time + 40, 15 + randomFluctuation.bounded(-2, 2));
                customPlot->graph(0)->addData(time + 48, 0);
            }
            counter++;

            if (counter > 6) {
                counter = 0;

                time += currentInterval;

             }

            customPlot->xAxis->setRange(time - 500, time + 125);
            double maxAmplitude = 100.0;  // Temporarily limiting max amp to 100
            customPlot->yAxis->setRange(-50, maxAmplitude);
            customPlot->replot();

        }
        else if (heartState == VTAC || heartState == VFIB){
            // Hardcoding datapoints for a VTAC/VFIB heartbeat
            if (counter == 0){
                customPlot->graph(0)->addData(time, 0);
                ui->bpmLabel->setStyleSheet("color: black;");
            }
            else if (counter == 1){
                if (heartState == VTAC)
                    customPlot->graph(0)->addData(time + 40, 80 + randomFluctuation.bounded(-2, 2));
                else if (heartState == VFIB)
                    customPlot->graph(0)->addData(time + 40, QRandomGenerator::securelySeeded().bounded(-20, 150) + randomFluctuation.bounded(-2, 2));
                ui->bpmLabel->setStyleSheet("color: red;");
                ui->bpmLabel->setText(QString("♥ BPM: %1").arg(pAED->getSensor()->getHeartRate()));
            }

            counter++;

            if (counter > 1) {
                counter = 0;

                time += currentInterval;

             }

            customPlot->xAxis->setRange(time - 500, time + 125);
            double maxAmplitude = 100.0;  // Temporarily limiting max amp to 100
            customPlot->yAxis->setRange(-50, maxAmplitude);

            customPlot->replot();
        }
        else if (heartState == ASYS || heartState == PEA){
            customPlot->graph(0)->addData(time, 0);
            ui->bpmLabel->setStyleSheet("color: black;");
            time += currentInterval;
            customPlot->xAxis->setRange(time - 500, time + 125);
            double maxAmplitude = 100.0;  // Temporarily limiting max amp to 100
            customPlot->yAxis->setRange(-50, maxAmplitude);

            customPlot->replot();
        }
    }


    currentInterval = pAED->generateInterval();
    if (currentInterval < 0)
        currentInterval = 70; //This fixes a bug with ASYS/PEA

}



void MainWindow::enableShockPB(){
    ui->shockPB->setEnabled(true);
}

void MainWindow::setPatientWindow(PatientWindow* pw){
    patientWindow = pw;
}

Patient* MainWindow::getpatient()
{
    if(pPatient) return pPatient;
    return nullptr;
}

void MainWindow::generateNewPatient()
{
    //creates random data for a new patient and assigns it to the pointer in class
    Patient *tempP = new Patient(69, "Bob");
    //delete patientRef;
    pPatient = tempP;
}

void MainWindow::communicateNewPatient()
{
    //calls the setter for all the functions that need to get the updated patient

    //TODO: Call the Connected function for setting patient to the SENSOR here
    //TODO: Pass the new Patient to patient window here

    pAED->getSensor()->setPatient(pPatient);
    patientWindow->setPatient(pPatient);
}


void MainWindow::initPatient()
{
    //deals with all the calls for creating and communicating the new patient
    generateNewPatient();
    communicateNewPatient();
}


MainWindow::~MainWindow()
{
    delete ui;
}


AED* MainWindow::getAed()
{
    if(pAED) return pAED;
    return nullptr;
}

void MainWindow::updateGUI(){
    //qDebug() << "Current GUI heartrate is " << pAED->getSensor()->getHeartRate();
}

void MainWindow::updateText(std::string s){
    QString qs = QString::fromStdString(s);
    if (qs != lastString){
        ui->textBrowser->append(qs);
        lastString = qs;
    }
}

void MainWindow::updateState(int state){
    int indicatorNum = state == -1 ? 0 : state;
    QString rbName = QString("radioButton") + QString::number(indicatorNum);
    QRadioButton* rb = findChild<QRadioButton*>(rbName);
    rb->setChecked(true);
}

void MainWindow::updateBatteryStatus(int value){
    ui->batteryStatus->setValue(value);
}


