#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "model.h"
#include "searchbar.h"
#include "ui_mainwindow.h"
#include "linechart.h"
#include <QtWidgets>
#include <QSlider>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(Model* model, QWidget *parent = 0);
    void updateRoadPreview();
public slots:
    void updateItem();
    void switchToRoadPage();
    void switchToMainPage();
    void switchToWeatherPage();
    void createCards();
    void updateCard();

    void updateView();
    void updateTimeLineLabel(int);
    void sendUpdateRequestForTimeline();
    void sendUpdateRequestForCoordinates();
signals:
    void itemClicked();
    void updateTimeline(int);
    void updateCoordinates(util::Coord);
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

//RoadPreview elements:
    QPushButton* prevButton_;
    QPushButton* nextButton_;
    //RoadPreview card elements:
        std::vector<QString> cards_;
        std::vector<CardsWidget*>  cardcontainer_;
        int index_;

//RoadPage elements:
     LineChart* roadDataChart;
     //Settings:
        QSlider* timeLineSlider;
        QLabel* timeLineLabel;
        QCheckBox* temperatureSelectionRP;
        QCheckBox* windSelectionRP;
        QCheckBox* cloudSelectionRP;
        QCheckBox* rainSelectionRP;
        QCheckBox* frictionSelectionRP;
        QCheckBox* trafficSelectionRP;


//WeatherPage elements:
        //Settings
        QSlider* timeLineSliderWP;
        QLabel* timeLineLabelWP;



// Search bar
    QWidget* searchBar;
    SearchBar* sb_;
};

#endif
