#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "model.h"
#include "ui_mainwindow.h"
#include "chart.h"
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

    void updateRoadDataChart();
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

//RoadPreview elements:
    QPushButton* prevButton_;
    QPushButton* nextButton_;
    //RoadPreview card elements:
        std::vector<QString> cards_;
        std::vector<CardsWidget*>  cardcontainer_;
        int index_;

//RoadPage elements:
        chart* roadDataChart;
        //Settings:
            QSlider* timeLineSlider;
            QLabel* timeLineLabel;


};

#endif
