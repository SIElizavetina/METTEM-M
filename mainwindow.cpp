#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setChart();

    ui->dial->setFocusPolicy(Qt::StrongFocus);
    ui->dial->setRange(-212,0);
    ui->dial->setNotchesVisible(true);

    file.open("data.txt");
    if (!file) exit(1);

    iter = 0;
    tmr = new QTimer(this);
    connect(tmr, SIGNAL(timeout()), this, SLOT(startTime()));
    tmr->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::setChart()
{

    //амперы
    series->setName("Ток тяговой батареи");
    seriesD->setName("Ток тяговой батареи сглаженый");

    chart->addSeries(series);
    chart->addSeries(seriesD);

    chart->addAxis(AxisX, Qt::AlignBottom);
    chart->addAxis(AxisY, Qt::AlignLeft);
    series->attachAxis(AxisX);
    series->attachAxis(AxisY);
    seriesD->attachAxis(AxisX);
    seriesD->attachAxis(AxisY);

    AxisX->setRange(0, 80);
    AxisY->setRange(-520, 520);

    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->resize(800, 500);
    chartView->setChart(chart);

    ui->ChartLayout->addWidget(chartView);

    //градусы
    seriesG->setName("Градусы");

    chartG->addSeries(seriesG);

    chartG->addAxis(AxisXG, Qt::AlignBottom);
    chartG->addAxis(AxisYG, Qt::AlignLeft);
    seriesG->attachAxis(AxisXG);
    seriesG->attachAxis(AxisYG);

    AxisXG->setRange(0, 80);
    AxisYG->setRange(-212-93, -93);

    QChartView *chartViewG = new QChartView();
    chartViewG->setRenderHint(QPainter::Antialiasing, true);
    chartViewG->resize(800, 500);
    chartViewG->setChart(chartG);

    ui->ChartLayout->addWidget(chartView);
    ui->ChartLayout->addWidget(chartViewG);
}

void MainWindow::startTime()
{
    //будем получать значение тока каждые 500мс
    std::string str;
    file >> str;
    if(str.size())
    {
        QString qstr = QString::fromStdString(str);

        //вычислим ток по формуле 4*N-500 и добавим значение в вектор
        amperage.push_back(4*qstr.toInt()-500);

        //демпфирование входной последовательности
        //за счет усреднения i-ого элемента с учетом предыдущих двух
        if(iter>1) amperageDemp.push_back((amperage[iter-2] + amperage[iter-1] +amperage[iter])/3);
        else amperageDemp.push_back(amperage[iter]);

        //сглаженые значения представим в градусах, исходя из того,
        //что диапазон шкалы  от 0 до -212, при этом присутствует сдвиг 93
        int From1=-500, From2=520;
        int To1=-212, To2=0;

        int t = ((double)amperageDemp[iter]-From1)/(From2-From1)*(To2-To1)+To1;
        grad.push_back(t-93);

        //полученные значение выведем с помощью графиков и шкалы
        *series<<QPointF(iter,amperage[iter]);
        *seriesD<<QPointF(iter,amperageDemp[iter]);
        *seriesG<<QPointF(iter,grad[iter]);
        ui->dial->setValue(grad[iter]+93);

        ++iter;
    }
    else
    {
        ui->dial->setValue(-212);
        tmr->stop();
    }
}
