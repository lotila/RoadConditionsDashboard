#ifndef CHART_H
#define CHART_H

#include <QtWidgets>

#include <QtCharts/QLineSeries>

struct point2d
{
    point2d(float X, float Y) : x(X), y(Y) {};

    float x;
    float y;

};


class Chart
{
public:
    /**
    * @param title
    * @param QT UI layoutWidget defines location and size of the chart.
    */
    Chart(QString& title, QWidget* layoutWidget);
    virtual ~Chart() {};

    /**
     * @brief vectorToQLineSeries
     * @param vector of points
     * @return QLineSeries*
     */
    static QLineSeries* vectorToQLineSeries(std::vector<point2d>& points);



protected:
    QString title_;
    QWidget* layoutWidget_;
};

#endif // CHART_H
