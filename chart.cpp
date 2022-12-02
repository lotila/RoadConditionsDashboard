#include "chart.h"


Chart::Chart(QString &title, QWidget* layoutWidget)
{
    title_ = title;
    layoutWidget_ = layoutWidget;
}


QLineSeries* Chart::vectorToQLineSeries(std::vector<util::TimeValuePair>& points)
{
    QLineSeries* series = new QLineSeries();
    for (util::TimeValuePair& point : points)
    {
        series->append(point.time, point.value);
    }
    return series;
}
