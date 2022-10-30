#include "mainwindow.h"
#include "chart.h"
#include "model.h"
#include "cardswidget.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <iostream>
#include <string>
#include <qwidget.h>

MainWindow::MainWindow(Model* model, QWidget* parent) :
    QWidget(parent), model(model)
{
    Ui::Form ui; //Initializing UI from ui_mainwindow.h
    ui.setupUi(this); //Setting the ui

    //Initializing the Stackedwidget for MainWindow:
    stackWidget = ui.stackedWidget;
    cardStackWidget = ui.stackedWidget_2;
    //Initializing the *page variables for MainWindow:
    mainpage = ui.mainPage;
    roadpage = ui.roadPage;
    weatherpage = ui.weatherPage;
    //Setting up the page to mainpage
    ui.stackedWidget->setCurrentWidget(mainpage);
//RoadFrame:
    //Initializing the Buttons for road Preview:
    nextButton_ = ui.nextButton_road;
    prevButton_ = ui.prevButton_road;

    //Connecting the prev and next buttons with updatefunction:
    connect(nextButton_, SIGNAL(clicked()), SLOT(updateCard()));
    connect(prevButton_, SIGNAL(clicked()), this, SLOT(updateCard()));

    //Initializing cards:
    cards_ = {"Friction", "Overall road condition", "Visibility"}; //Currntly this is the DATA tells the card headers and amount of cards to be created
    index_ = 0; //Setting the current card(index 0)
    cardcontainer_ = {};
    createCards();

    //this->cardStackWidget->setCurrentIndex(index_);
    this->cardStackWidget->setCurrentWidget(cardcontainer_.at(index_));

//WeatherFrame:
    //Old elements to the Weather previewFrame:
    QFrame *weatherFrame = ui.weather_frame;
    //Adding the old stuff to the weather frame:
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->view = new QLabel(this->model->weather->avgWind({0, 0}, {1, 2}).c_str());
    this->view->setAlignment(Qt::AlignCenter);
    this->view->setStyleSheet("font: 20pt;");
    layout->addWidget(this->view);
    this->otherButton = new QPushButton("Update", this);
    layout->addWidget(this->otherButton);
    weatherFrame->setLayout(layout);
    //Bringing the more button to front:
    ui.moreButton_weather->raise();

//RoadPage:
    chart* cardChart = new chart("esimerkki kaavio", ui.roadChartWidget);
   // cardChart->XaxisLabel("esim akseli X");
    //cardChart->YaxisLabel("esim akseli Y");

    std::vector<point2d> data = {point2d(1,2), point2d(2,3),point2d(3,1),point2d(4,0),point2d(5,2)};
    std::vector<point2d> data2 = {point2d(1,2), point2d(2,3),point2d(3,4),point2d(9,0),point2d(17,4)};

    cardChart->newPlot("esimerkki kuvaaja" ,data);
    cardChart->newPlot("esimerkki kuvaaja 2" ,data2);

//Connections for Buttons in GUI:
    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(ui.moreButton_road, SIGNAL(clicked()), this, SLOT(switchToRoadPage()));
    connect(ui.moreButton_weather, SIGNAL(clicked()), this, SLOT(switchToWeatherPage()));
    connect(ui.homeButton_road, SIGNAL(clicked()), this, SLOT(switchToMainPage()));
    connect(ui.homeButton_weather, SIGNAL(clicked()), this, SLOT(switchToMainPage()));



}
void MainWindow::updateItem()
{
    this->view->setText(this->model->weather->avgWind({0, 0}, {1, 2}).c_str());
}
void MainWindow::createCards()
{
    for(int i = 0; i < cards_.size(); i++)
    {
        CardsWidget* createdCard = new CardsWidget(cards_.at(i), 80); //Creating cards with Variable name and int value
        this->cardcontainer_.push_back(createdCard);
        this->cardStackWidget->addWidget(createdCard);
    }
}

//Updating the index of a card currently shown to you
void MainWindow::updateCard()
{
    //Check the sender:
    QObject* obj = QObject::sender();
    //Sender was nextButton
    if(obj == this->nextButton_)
    {   //If the index gets out of bounds:
        if(index_ + 1 > cards_.size()-1)
        {
            index_ = 0;
            this->cardStackWidget->setCurrentWidget(cardcontainer_.at(index_));
            return;
         }
        index_ += 1;
    }
    //Sender was previousButton:
    if(obj == this->prevButton_)
    {
        //If the index gets out of bounds:
                if(index_ - 1 < 0)
                {
                    index_ = cards_.size()-1;
                    this->cardStackWidget->setCurrentWidget(cardcontainer_.at(index_));
                    return;
                 }
                index_ -= 1;
    }
    this->cardStackWidget->setCurrentWidget(cardcontainer_.at(index_));
}
void MainWindow::switchToRoadPage()
{
    this->stackWidget->setCurrentWidget(this->roadpage);
}
void MainWindow::switchToMainPage()
{
    this->stackWidget->setCurrentWidget(this->mainpage);
}
void MainWindow::switchToWeatherPage()
{
    this->stackWidget->setCurrentWidget(this->weatherpage);
}
