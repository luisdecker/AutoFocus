#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T13:56:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
TARGET = AutoFocusMethods
TEMPLATE = app


#Includes OpenCV========================
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree
#=======================================

SOURCES += main.cpp\
        mainwindow.cpp \
    clickhandler.cpp \
    squarewindow.cpp \
    autofocusmethods.cpp \
    focusmetrics.cpp

HEADERS  += mainwindow.h \
    clickablelabel.h \
    clickhandler.h \
    imageconverter.h \
    squarewindow.h \
    rectanglewatcher.h \
    autofocusmethods.h \
    focusmetrics.h

FORMS    += mainwindow.ui \
    squarewindow.ui
