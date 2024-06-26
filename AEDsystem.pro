QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aed.cpp \
    analyzer.cpp \
    patientwindow.cpp \
    qcustomplot.cpp \
    sensor.cpp \
    patient.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    aed.h \
    analyzer.h \
    enums.h \
    patientwindow.h \
    qcustomplot.h \
    sensor.h \
    patient.h \
    mainwindow.h

FORMS += \
    mainwindow.ui \
    patientwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images/Images.qrc
