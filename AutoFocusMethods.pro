#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T13:56:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS += -Wno-unused-parameter

QMAKE_CXXFLAGS += -fopenmp
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
LIBS += -lopencv_flann
LIBS += -lopencv_imgcodecs
#LIBS += -lopencv_nonfree
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#=======================================

#Includes OpenMP========================
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
#=======================================

SOURCES += main.cpp\
        mainwindow.cpp \
    clickhandler.cpp \
    autofocusmethods.cpp \
    focusmetrics.cpp \
    hologramdecoder.cpp \
    ComplexMatrix.cpp \
    imagehistogram.cpp \
    comparative.cpp

HEADERS  += mainwindow.h \
    clickablelabel.h \
    clickhandler.h \
    imageconverter.h \
    rectanglewatcher.h \
    autofocusmethods.h \
    focusmetrics.h \
    hologramdecoder.h \
    ComplexMatrix.h \
    imagehistogram.h

FORMS    += mainwindow.ui
