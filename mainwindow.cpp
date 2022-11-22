#include "mainwindow.h"
#include "chart.h"
#include "model.h"
#include "cardswidget.h"
#include "searchbar.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <iostream>
#include <string>
#include <qwidget.h>

MainWindow::MainWindow(Model* model, QWidget* parent) :
    QWidget(parent), model(model)
{
    this->ui.setupUi(this); //Setting the ui


    //Initializing the Stackedwidget for MainWindow:
    this->stackWidget = ui.stackedWidget;
    this->cardStackWidget = ui.stackedWidget_2;
    //Initializing the *page variables for MainWindow:
    this->mainpage = ui.mainPage;
    this->roadpage = ui.roadPage;
    this->weatherpage = ui.weatherPage;
    //Setting up the page to mainpage
    ui.stackedWidget->setCurrentWidget(mainpage);

    // Setting up the search bar
    searchBar = ui.searchWidgetMain;
    sb_ = new SearchBar(searchBar);

//RoadFrame:
    //Initializing the Buttons for road Preview:
    this->nextButton_ = ui.nextButton_road;
    this->prevButton_ = ui.prevButton_road;

    //Connecting the prev and next buttons with updatefunction:
    connect(nextButton_, SIGNAL(clicked()), SLOT(updateCard()));
    connect(prevButton_, SIGNAL(clicked()), this, SLOT(updateCard()));

    //Initializing cards:
    this->cards_ = {"Friction", "roadcondition", "Visibility",}; //Currntly this is the DATA tells the card headers and amount of cards to be created
    this->index_ = 0; //Setting the current card(index 0)
    this->cardcontainer_ = {};
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
   this->roadDataChart = new chart("esimerkki kaavio", ui.roadChartWidget);
   // cardChart->XaxisLabel("esim akseli X");
   //cardChart->YaxisLabel("esim akseli Y");

    //This is data to feed the advanced Roadchart. This should come from thee model in the future:
    std::vector<point2d> data = {point2d(1,2), point2d(2,3),point2d(3,1),point2d(4,0),point2d(5,2)};
    std::vector<point2d> data2 = {point2d(1,2), point2d(2,3),point2d(3,4),point2d(9,0),point2d(17,4)};
    //Plotting the wanted data:
    this->roadDataChart->newPlot("esimerkki kuvaaja" ,data);
    this->roadDataChart->newPlot("esimerkki kuvaaja 2" ,data2);
    //Initialize chart setting elements:
    this->timeLineSlider = this->ui.timeLineSlider;
    this->timeLineLabel = this->ui.timeLineSelected;
    this->timeLineSlider->setRange(-12, 12);

    this->timeLineLabel->setText("0");
    //update the Timeline position actively
    connect(this->timeLineSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTimeLineLabel(int)));
    //Update model when timeline is selected(handle is released):
    connect(this->timeLineSlider, SIGNAL(sliderReleased()), this, SLOT(sendUpdateRequestForRoadData()));
//WeatherPage:

    chart* weatherChart = new chart("esimerkki kaavio", ui.weatherChart);
   // cardChart->XaxisLabel("esim akseli X");
    //cardChart->YaxisLabel("esim akseli Y");

    std::vector<point2d> weatherData = {point2d(1,2), point2d(2,3),point2d(3,1),point2d(4,0),point2d(5,2)};
    std::vector<point2d> weatherData2 = {point2d(1,2), point2d(2,3),point2d(3,4),point2d(9,0),point2d(17,4)};

    weatherChart->newPlot("esimerkki kuvaaja" ,weatherData);
    weatherChart->newPlot("esimerkki kuvaaja 2" ,weatherData2);

//Connections for Buttons in GUI:

    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(ui.moreButton_road, SIGNAL(clicked()), this, SLOT(switchToRoadPage()));
    connect(ui.moreButton_weather, SIGNAL(clicked()), this, SLOT(switchToWeatherPage()));
    connect(ui.homeButton_road, SIGNAL(clicked()), this, SLOT(switchToMainPage()));

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
void MainWindow::updateRoadDataChart()
{ //Here we should get the data from model and give it to our chart from those variables that are selected:

}
void MainWindow::updateTimeLineLabel(int newValue)
{
    QString timeLineText = QString::number(newValue);
    this->timeLineLabel->setText(timeLineText);
}

void MainWindow::sendUpdateRequestForRoadData()
{
    int timePos = this->timeLineSlider->sliderPosition();
    emit this->updateRoadData(timePos);
}
