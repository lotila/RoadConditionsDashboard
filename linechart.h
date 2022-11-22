#ifndef LINECHART_H
#define LINECHART_H


/*
 * Class for creating a chart for QT UI
 *
// Example use:
-------------

    lineChart* windSpeedChart = new lineChart("esimerkki kaavio", ui->horizontalLayoutWidget);

    windSpeedChart->XaxisLabel("esim akseli X");
    windSpeedChart->YaxisLabel("esim akseli Y");

    std::vector<point2d> data = {point2d(1,2), point2d(2,3),point2d(3,1),point2d(4,0),point2d(5,2)};
    std::vector<point2d> data2 = {point2d(1,2), point2d(2,3),point2d(3,4),point2d(9,0),point2d(17,4)};

    windSpeedChart->newPlot("esimerkki kuvaaja" ,data);
    windSpeedChart->newPlot("esimerkki kuvaaja 2" ,data2);

*/

#include <vector>

#include <QtWidgets>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QCategoryAxis>

#include "chart.h"


class LineChart : public Chart
{
public:
    /**
    * @param title
    * @param QT UI layoutWidget defines location and size of the chart.
    */
    LineChart(QString title, QWidget* layoutWidget);
    ~LineChart();


    /**
     * @brief XaxisLabel
     * @param name
     */
    void XaxisLabel(QString name);
    /**
     * @brief YaxisLabel
     * @param name
     */
    void YaxisLabel(QString name);

    /**
     * @brief newPlot
     * @param name
     * @param vector of plot points
     */
    void newPlot(QString name, std::vector<point2d>& points );

    void hide() const;
    void show() const;


    /**
     * @brief deletePlots, new plots can be added after
     */
    void deletePlots();



private:

    QValueAxis *axisX;
    QValueAxis *axisY;

    std::vector<QLineSeries*> series_;
    QChartView* chartView_;
    QChart* chart_;

};

#endif // LINECHART_H
