#-------------------------------------------------
#
# Project created by QtCreator 2013-12-17T14:16:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = monores
TEMPLATE = app
VERSION  = 0.1.0

QMAKE_LFLAGS   += -std=c++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wold-style-cast -Wall -Wuninitialized -Wextra
QMAKE_CXXFLAGS_RELEASE += -funroll-loops # unroll short, iterative for-loops
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_DEBUG   += -O0

contains( QMAKE_CC, gcc ) {
    # http://gcc.gnu.org/bugzilla/show_bug.cgi?id=55805
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers
}

SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.hpp \
            qcustomplot/qcustomplot.hpp

INCLUDEPATH += qcustomplot

FORMS    += mainwindow.ui
