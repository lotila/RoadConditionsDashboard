
#include "chart.h"


Chart::Chart(QString &title, QWidget* layoutWidget)
{
    title_ = title;
    layoutWidget_ = layoutWidget;
}


QLineSeries* Chart::vectorToQLineSeries(std::vector<point2d>& points)
{
    QLineSeries* series = new QLineSeries();
    for (point2d& point : points)
    {
        series->append(point.x, point.y);
    }
    return series;
}

