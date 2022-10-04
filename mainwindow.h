#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qchart.h>
#include <cmath>
#include <fstream>

#include <QWidget>
#include <QLayout>
#include <QTimer>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QValueAxis>
#include <QtCharts/QAreaSeries>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QVector<int> amperage;//вектор, хранящий входной сигнал
    QVector<int> amperageDemp;//вектор, хранящий значения входного тока
    QVector<int> grad;//вектор, хранящийзначения в градусах

    void setChart();//установка графика
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();
    QLineSeries *seriesD = new QLineSeries();
    QValueAxis *AxisX = new QValueAxis();
    QValueAxis *AxisY = new QValueAxis();

    QChart *chartG = new QChart();
    QLineSeries *seriesG = new QLineSeries();
    QValueAxis *AxisXG = new QValueAxis();
    QValueAxis *AxisYG = new QValueAxis();

    std::ifstream file;

    QTimer * tmr;
    int iter;

private slots:
    void  startTime();
};
#endif // MAINWINDOW_H
