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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>

class QCustomPlot;
class QTimer;



#include <array>


const size_t g_level = 10;
const size_t N = (1 << g_level) + 1;

typedef double real;
typedef std::array<real,N> Data;
typedef QVector<real> Vector;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    QCustomPlot *customPlot;
    QCustomPlot *customPlot2;
    QTimer *timer;

    Data data_init;

    Vector xvalues;
    Vector yvalues;
    Vector yvalues_th;
    Vector tvalues;

    std::array<Vector,3> errorvalues;

    Data data, data2;

    size_t g_clock = 0;

private slots:
    void initSolver();
    void timeStep();
    void fillError();
    void replot();

    void autoPlayToggled(bool checked);
};

#endif // MAINWINDOW_HPP
