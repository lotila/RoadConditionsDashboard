#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "model.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

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
signals:
    void itemClicked();

private:
    //WeatherPreview elements:
    QLabel* view;
    QPushButton *button;
    QPushButton *otherButton;
    //model:
    Model* model;
    //Ui:
    Ui::Form ui;
    //Pages and stackedwidget:
    QWidget* mainpage;
    QWidget* roadpage;
    QWidget* weatherpage;
    QStackedWidget* stackWidget;
    QStackedWidget* cardStackWidget;

    //RoadPreview Card variables:
    std::vector<QString> cards_;
    std::vector<CardsWidget*>  cardcontainer_;
    int index_;
    QLabel* now_;
    QLabel* predicted_;

    QPushButton* prevButton_;
    QPushButton* nextButton_;

};

#endif
