#include "linechart.h"


LineChart::LineChart(QString title,  QWidget* layoutWidget)
    :  Chart(title, layoutWidget)
{
    layoutWidget_ = layoutWidget;
    chart_ = new QChart();
    chartView_ = new QChartView(chart_);

    chart_->setTitle(title);
    chart_->legend()->setVisible(true);
    chart_->legend()->setAlignment(Qt::AlignBottom);
    chart_->legend()->setInteractive(true);
    chart_->setAnimationOptions(QChart::AllAnimations);

    chartView_->setRenderHint(QPainter::Antialiasing);
    chartView_->setParent(layoutWidget_);
    chartView_->setFixedSize(layoutWidget_->size());
}
LineChart::~LineChart()
{
    chart_->removeAllSeries();
    delete chart_;
    series_.clear();
    delete chartView_;
}

void LineChart::XaxisLabel(QString name)
{
    axisX = new QValueAxis();

    axisX->setTickCount(1);
    axisX->setLabelFormat("%.2f");

    axisX->setTitleText(name);
    chart_->addAxis(axisX, Qt::AlignBottom);

}

void LineChart::YaxisLabel(QString name)
{
    axisY = new QValueAxis();

    axisY->setTickCount(1);
    axisY->setLabelFormat("%.2f");

    axisY->setTitleText(name);

    chart_->addAxis(axisY, Qt::AlignLeft);


}

void LineChart::newPlot(QString name, std::vector<util::TimeValuePair> points )
{

   series_.push_back(Chart::vectorToQLineSeries(points));
   series_.back()->setName(name);

   chart_->addSeries(series_.back());
   chart_->createDefaultAxes();

}

void LineChart::hide() const {chartView_->hide();}

void LineChart::show() const {chartView_->show();}

void LineChart::deletePlots()
{
    chart_->removeAllSeries();
}


