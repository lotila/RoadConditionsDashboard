#ifndef CHART_H
#define CHART_H

/*
 * Class for creating a chart for QT UI
 *
// Example use:
-------------

    chart* windSpeedChart = new chart("esimerkki kaavio", ui->horizontalLayoutWidget);

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


struct point2d
{
    point2d(float X, float Y) : x(X), y(Y) {};

    float x;
    float y;

};


class chart
{
public:
    /**
     * @param title
     * @param QT UI layoutWidget defines location and size of the chart.
     */
    chart(QString title, QWidget* layoutWidget);
    ~chart();


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

    void hide();
    void show();


    /**
     * @brief deletePlots, new plots can be added after
     */
    void deletePlots();

    /**
     * @brief vectorToQLineSeries
     * @param vector of points
     * @return QLineSeries*
     */
    static QLineSeries* vectorToQLineSeries(std::vector<point2d>& points);


private:

    QValueAxis *axisX;
    QValueAxis *axisY;

    std::vector<QLineSeries*> series_;
    QChartView* chartView_;
    QChart* chart_;
    QWidget* layoutWidget_;

};

#endif // CHART_H
