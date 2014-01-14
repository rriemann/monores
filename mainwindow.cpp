/***************************************************************************************
 * Copyright (c) 2013 Robert Riemann <robert@riemann.cc>                                *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

#include <numeric>



const real x0    = -1.0;
const real x1    = +1.0;
const real span  = x1-x0;
const real dx    = span/(N);
const real dx2   = 2*dx;

real f_eval_gauss(real x) {
    x = std::fmod((x - x0), span) + x0;
    if(x < x0) {
        x += span;
    }
    return exp(-10*x*x);
}

const real g_cfl  = 0.2;
const real g_velocity = 0.5;
// https://en.wikipedia.org/wiki/Courant%E2%80%93Friedrichs%E2%80%93Lewy_condition
const real dt = g_cfl*dx/g_velocity;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setInterval(100);

    connect(ui->actionAutoPlay, SIGNAL(triggered(bool)), this, SLOT(autoPlayToggled(bool)));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(initSolver()));
    connect(ui->actionStep, SIGNAL(triggered()), this, SLOT(timeStep()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timeStep()));

    customPlot = ui->customPlot;
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    customPlot2 = ui->customPlot2;
    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // initialization
    for(size_t i = 0; i < N; ++i) {
        real x = i*dx+x0;
        xvalues << x;
        data_init[i] = f_eval_gauss(x);
    }

    qDebug() << QString("max level: %1").arg(g_level);

    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::black));
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(x0, x1);
    // customPlot->yAxis->setRange(-0.2, 1.2);

    customPlot2->addGraph();
    customPlot2->graph(0)->setPen(QPen(Qt::black));
    customPlot2->graph(0)->setName("L_1");
    customPlot2->addGraph();
    customPlot2->graph(1)->setPen(QPen(Qt::red));
    customPlot2->graph(1)->setName("L_2");
    customPlot2->addGraph();
    customPlot2->graph(2)->setPen(QPen(Qt::blue));
    customPlot2->graph(2)->setName("L_inf");
    // customPlot2->yAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot2->xAxis->setLabel("t");
    customPlot2->yAxis->setLabel("error");
    customPlot2->legend->setVisible(true);

    initSolver();
}

void MainWindow::initSolver()
{
    for(Vector &v: errorvalues) {
        v.clear();
    }
    yvalues.clear();
    tvalues.clear();

    data  = data_init;
    data2 = data_init;

    g_clock = 0;

    replot();
}

void MainWindow::timeStep()
{
    for(size_t i = 0; i < 4; ++i) {
        data[0] = data2[0] - g_velocity*dt*(data2[1]-data2[N-1])/dx;
        for(size_t j = 1; j < N-1; ++j){
            real derivative = (data2[j+1]-data2[j-1])/dx2;
            data[j] = data2[j] - g_velocity*dt*derivative;
        }
        data[N-1] = data2[N-1] - g_velocity*dt*(data2[0]-data2[N-2])/dx;
        data2 = data;

        g_clock++;
    }

    replot();
}

void MainWindow::fillError()
{
    tvalues << g_clock*dt;

    real offset = g_clock*dt*g_velocity;
    real error_1 = 0;
    real error_2 = 0;
    Data error_3;
    yvalues_th.clear();
    for(size_t j = 0; j < N; ++j) {
        // acurate solution
        real value_th = f_eval_gauss(j*dx-offset+x0);
        yvalues_th << value_th;
        real diff = fabs(value_th-data[j]);
        error_1 += diff;
        error_2 += pow(diff,2);
        error_3[j] = diff;
    }

    errorvalues[0] << error_1;
    errorvalues[1] << sqrt(error_2);
    errorvalues[2] << * std::max_element(error_3.begin(), error_3.end());
}

void MainWindow::replot()
{
    fillError();

    yvalues.clear();
    for(const real &y: data) {
        yvalues << y;
    }

    customPlot->graph(0)->setData(xvalues, yvalues);
    customPlot->graph(1)->setData(xvalues, yvalues_th);

    customPlot->rescaleAxes();
    customPlot->replot();

    for(size_t i = 0; i < errorvalues.size(); ++i) {
        customPlot2->graph(i)->setData(tvalues, errorvalues[i]);
    }
    customPlot2->rescaleAxes();
    customPlot2->replot();
}

void MainWindow::autoPlayToggled(bool checked)
{
    if(checked) {
        timer->start();
    } else {
        timer->stop();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
