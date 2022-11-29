#ifndef CHART_H
#define CHART_H

#include <QtWidgets>

#include <QtCharts/QLineSeries>
#include "util.h"


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
    static QLineSeries* vectorToQLineSeries(std::vector<util::TimeValuePair>& points);



protected:
    QString title_;
    QWidget* layoutWidget_;
};

#endif // CHART_H
