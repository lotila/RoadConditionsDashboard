
#include "chart.h"

chart::chart(QString title,  QWidget* layoutWidget)
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
chart::~chart()
{
    chart_->removeAllSeries();
    delete chart_;
    series_.clear();
    delete chartView_;
}

void chart::XaxisLabel(QString name)
{
    axisX = new QValueAxis;

    axisX->setTickCount(1);
    axisX->setLabelFormat("%.2f");

    axisX->setTitleText(name);
    chart_->addAxis(axisX, Qt::AlignBottom);

}

void chart::YaxisLabel(QString name)
{
    axisY = new QValueAxis;

    axisY->setTickCount(1);
    axisY->setLabelFormat("%.2f");

    axisY->setTitleText(name);

    chart_->addAxis(axisY, Qt::AlignLeft);


}

void chart::newPlot(QString name, std::vector<point2d>& points )
{

   series_.push_back(vectorToQLineSeries(points));
   series_.back()->setName(name);

   chart_->addSeries(series_.back());
   chart_->createDefaultAxes();

}

void chart::hide() {chartView_->hide();}

void chart::show() {chartView_->show();}

void chart::deletePlots()
{
    chart_->removeAllSeries();
}


QLineSeries* chart::vectorToQLineSeries(std::vector<point2d>& points)
{
    QLineSeries* series = new QLineSeries();
    for (point2d& point : points)
    {
        series->append(point.x, point.y);
    }
    return series;
}

