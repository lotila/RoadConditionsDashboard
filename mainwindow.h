#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "model.h"
#include "searchbar.h"
#include "ui_mainwindow.h"
#include "linechart.h"
#include "piechart.h"
#include <QtWidgets>
#include <QSlider>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(Model* model, QWidget *parent = 0);
    void updateRoadPreview();
    void setWeatherIcons();

public slots:
    void updateItem();
    void switchToRoadPage();
    void switchToMainPage();
    void switchToWeatherPage();
    void createCards();
    void updateCard();
    void updateCardInfo();

    void updateRoadDataChart();


    void updateWeatherChartData();
    void updateWeatherData();

    void updateTimeLineLabel(int);
    void sendUpdateRequestForRoadData();

signals:
    void itemClicked();
    void updateRoadData(int);

private:
//Model:
    Model* model;
//Ui:
    Ui::Form ui;
//Pages and stackedwidget:
    QWidget* mainpage;
    QWidget* roadpage;
    QWidget* weatherpage;
    QStackedWidget* stackWidget;
    QStackedWidget* cardStackWidget;
//WeatherPreview elements:
    QLabel* view;
    QPushButton *button;
    QPushButton *otherButton;

    // WeatherPage elements:
    QLabel *sunIcon_;
    QLabel *cloudIcon_;
    QLabel *windIcon_;
    QLabel *snowIcon_;
    QLabel *rainIcon_;

    LineChart *weatherPageChart_;

//RoadPreview elements:
    QPushButton* prevButton_;
    QPushButton* nextButton_;
    //RoadPreview card elements:
        std::vector<QString> cards_;
        std::vector<CardsWidget*>  cardcontainer_;
        int index_;

//RoadPage elements:
        LineChart* roadDataChart;
        PieChart* pieChart;
        //Settings:
            QSlider* timeLineSlider;
            QLabel* timeLineLabel;

//weather page elements
            LineChart* weatherChart;


    // Search bar
    QWidget* searchBar;
    SearchBar* sb_;
};

#endif
