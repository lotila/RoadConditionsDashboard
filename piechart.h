#ifndef PIECHART_H
#define PIECHART_H

#include "chart.h"


#include <unordered_map>
#include <QtWidgets>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


class PieChart : public Chart
{
public:
    PieChart(QString title,  QWidget* layoutWidget, std::unordered_map<QString, int>& slices );
    ~PieChart();


private:
    QPieSeries *series_;

    QChart *chart_;
    QChartView *chartView_;
};

#endif // PIECHART_H
