#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "patient.h"
#include "aed.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



AED* getAed();


private slots:

    void on_pushButton1_clicked();


private:
    void updateGUI();

    Ui::MainWindow *ui;
    //Patient *patientRef;
    AED *aedRef;

};
#endif // MAINWINDOW_H
