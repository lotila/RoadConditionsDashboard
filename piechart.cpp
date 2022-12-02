#include "piechart.h"

PieChart::PieChart(QString title,  QWidget* layoutWidget,
                   std::unordered_map<std::string, int> slices )
    : Chart(title, layoutWidget)
{
    series_ = new QPieSeries();
    for (auto& slice : slices)
    {
        series_->append(QString::fromStdString(slice.first),slice.second);
    }

    for (auto& slice : series_->slices())
    {
        slice->setLabel(QString( slice->label() + " %1").arg(slice->value()));
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::black, 2));
    }


    chart_ = new QChart();
    chart_->addSeries(series_);
    chart_->setTitle(title);
    chart_->legend()->hide();


    chartView_ = new QChartView(chart_);
    chartView_->setRenderHint(QPainter::Antialiasing);
    chartView_->setParent(layoutWidget_);
    chartView_->setFixedSize(layoutWidget_->size());

}

PieChart::~PieChart()
{
    series_->clear();
    chart_->removeAllSeries();
    delete chart_;
    delete chartView_;
}

