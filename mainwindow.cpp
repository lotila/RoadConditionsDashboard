#include "mainwindow.h"
#include "model.h"
#include "cardswidget.h"
#include "searchbar.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <iostream>
#include <string>
#include <qwidget.h>
#include <unordered_map>

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

    // this->model->roadCondition->updateRoadCondition(util::Coord(sb_.lat, sb_.lon));

//RoadFrame:
    //Initializing the Buttons for road Preview:
    this->nextButton_ = ui.nextButton_road;
    this->prevButton_ = ui.prevButton_road;

    //Connecting the prev and next buttons with updatefunction:
    connect(nextButton_, SIGNAL(clicked()), SLOT(updateCard()));
    connect(prevButton_, SIGNAL(clicked()), this, SLOT(updateCard()));

    //Initializing cards:
    this->cards_ = {"Friction", "Roadcondition", "Visibility",}; //Currntly this is the DATA tells the card headers and amount of cards to be created
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
    this->view = new QLabel(QString::number(this->model->weather->getCurrentWind()));
    this->view->setAlignment(Qt::AlignCenter);
    this->view->setStyleSheet("font: 20pt;");
    layout->addWidget(this->view);
    this->otherButton = new QPushButton("Update", this);
    layout->addWidget(this->otherButton);
    weatherFrame->setLayout(layout);
    //Bringing the more button to front:
    ui.moreButton_weather->raise();

//RoadPage:
    this->roadDataChart = new LineChart("", ui.roadChartWidget);
    updateRoadDataChart();
    connect(this->ui.trafficMsgCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.cloudinessCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.rainCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.temperatureCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.frictionCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.windCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));

//Connections for ButBtons in GUI:

    //Initialize chart setting elements:
    this->timeLineSlider = this->ui.timeLineSlider;
    this->timeLineLabel = this->ui.timeLineSelected;
    this->timeLineSlider->setRange(-12, 12);

    //Initialize chart setting elements:

    this->timeLineSlider = this->ui.timeLineSlider; //Timeline slider
    this->timeLineLabel = this->ui.timeLineSelected; //Timeline value label
    this->timeLineSlider->setRange(-12, 12);
    this->timeLineLabel->setText("0");

    //Signals and slots:
    //update the Timeline value actively
    connect(this->timeLineSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTimeLineLabel(int)));
    //Update model when timeline is selected(handle is released):

    connect(this->timeLineSlider, SIGNAL(sliderReleased()), this, SLOT(sendUpdateRequestForTimeline()));

//WeatherPage:
    this->timeLineSliderWP = this->ui.timeLineSliderWP; //Timeline slider
    this->timeLineLabelWP = this->ui.timeLineSelectedWP; //Timeline value label
    this->timeLineSliderWP->setRange(-12, 12);
    this->timeLineLabelWP->setText("0");

    connect(this->timeLineSliderWP, SIGNAL(valueChanged(int)), this, SLOT(updateTimeLineLabel(int)));
    //Update model when timeline is selected(handle is released):
    connect(this->timeLineSliderWP, SIGNAL(sliderReleased()), this, SLOT(sendUpdateRequestForTimeline()));

    connect(this->ui.rainCheckBoxWeatherPage, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));
    connect(this->ui.windCheckBoxWeatherPage, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));
    connect(this->ui.temperatureCheckBoxWeatherPage, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));

//WeatherPage:
    this->weatherChart = new LineChart("", ui.weatherChart);
    //Plotting the wanted data:
    updateWeatherData();

    this->sunIcon_ = ui.sunIcon;
    this->cloudIcon_ = ui.cloudIcon;
    this->windIcon_ = ui.windIcon;
    this->snowIcon_ = ui.snowIcon;
    this->rainIcon_ = ui.rainIcon;

    setWeatherIcons();

//Connections for Buttons in GUI:
    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(ui.moreButton_road, SIGNAL(clicked()), this, SLOT(switchToRoadPage()));
    connect(ui.moreButton_weather, SIGNAL(clicked()), this, SLOT(switchToWeatherPage()));
    connect(ui.homeButton_road, SIGNAL(clicked()), this, SLOT(switchToMainPage()));

// maintanance work
    this->pieChart = new PieChart("", this->ui.maintananceWorkPie, this->model->maintenance->getMaintanance());

    connect(ui.searchButton, SIGNAL(pressed()), this, SLOT(sendUpdateRequestForCoordinates()));
}

void MainWindow::updateItem()
{
    this->view->setText(QString::number(this->model->weather->getCurrentWind()));
}

void MainWindow::createCards()
{
    for(int i = 0; i < cards_.size(); i++)
    {
        CardsWidget* createdCard = new CardsWidget(cards_.at(i), this->model); //Creating cards with Variable name and int value
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


void MainWindow::updateCardInfo()
{
    for ( CardsWidget* card :  this->cardcontainer_)
    {
        card->updateCardInfo();
    }
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
{
    this->roadDataChart->deletePlots();
    if (ui.windCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("wind" ,model->weather->getWind());
    if (ui.temperatureCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("temperature" ,model->weather->getTemperature());
    if (ui.rainCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("rain" ,model->weather->getRain());
    if (ui.cloudinessCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("Cloudiness" ,model->weather->getCloudiness());
    if (ui.trafficMsgCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("traffic messages" ,model->trafficMessages->getTrafficMessageCount());
    if (ui.frictionCheckboxRoadPage->isChecked()) this->roadDataChart->newPlot("friction" ,model->roadCondition->getFriction());
}
void MainWindow::updateView()
{ //Here we should draw the charts again


}

void MainWindow::updateWeatherChartData()
{
    this->weatherChart->deletePlots();
    if (ui.windCheckBoxWeatherPage->isChecked()) this->weatherChart->newPlot("wind" ,model->weather->getWind());
    if (ui.temperatureCheckBoxWeatherPage->isChecked()) this->weatherChart->newPlot("temperature" ,model->weather->getTemperature());
    if (ui.rainCheckBoxWeatherPage->isChecked()) this->weatherChart->newPlot("rain" ,model->weather->getRain());
    //this->weatherChart->newPlot("wind" ,model->weather->getCloudiness())
}

void MainWindow::updateWeatherData()
    {

    updateWeatherChartData();
    // show current weather conditions
    ui.lcdNumber->display(model->weather->getCurrentTemperature());
    ui.lcdNumber_2->display(model->weather->getCurrentWind());
    ui.lcdNumber_3->display(model->weather->getCurrentRain());
    // shouldn't this be here? model->weather->getCurrentCloudiness();

    // show key temperatures of the month
    ui.maxTempLcd->display(model->weather->getMaxTemp());
    ui.minTempLcd->display(model->weather->getMinTemp());
    ui.dailyAvgLcd->display(model->weather->getAverageTemp());
    }

void MainWindow::updateTimeLineLabel(int newValue)
{
    QObject* slider = QObject::sender();
    QString timeLineText = QString::number(newValue);
    if(slider == this->timeLineSlider) this->timeLineLabel->setText(timeLineText);
    if(slider == this->timeLineSliderWP) this->timeLineLabelWP->setText(timeLineText);
}
void MainWindow::sendUpdateRequestForTimeline()
{
    QObject* slider = QObject::sender();
    int value = 0;
//Update the timeline label for which evber slider send it
    if(slider == this->timeLineSlider) value= this->timeLineSlider->value();
    if(slider == this->timeLineSliderWP) value = this->timeLineSliderWP->value();
    emit this->updateTimeline(value);
}
void MainWindow::sendUpdateRequestForCoordinates()
{
    util::Coord newCoords = {22, 44}; //this->sb_->getCoordinates(); TODO: use sb methods
    emit this->updateCoordinates(newCoords);
}

void MainWindow::setWeatherIcons()
{
    /*
    QPixmap sun(":/images/sun.png");
    this->sunIcon_->setPixmap(sun);
    this->sunIcon_->setScaledContents(true);
    this->sunIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->sunIcon_->show();
    QPixmap cloud(":/images/cloud.png");
    this->cloudIcon_->setPixmap(cloud);
    this->cloudIcon_->setScaledContents(true);
    this->cloudIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->cloudIcon_->show();
    QPixmap rain(":/images/rain.png");
    this->rainIcon_->setPixmap(rain);
    this->rainIcon_->setScaledContents(true);
    this->rainIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->rainIcon_->show();
    QPixmap snow(":/images/snow.png");
    this->snowIcon_->setPixmap(snow);
    this->snowIcon_->setScaledContents(true);
    this->snowIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->snowIcon_->show();
    QPixmap wind(":/images/wind.png");
    this->windIcon_->setPixmap(wind);
    this->windIcon_->setScaledContents(true);
    this->windIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    this->windIcon_->show();
    */


    if(model->weather->getCurrentCloudiness() < 0.6) {
        QPixmap sun(":/images/sun.png");
        this->sunIcon_->setPixmap(sun);
        this->sunIcon_->setScaledContents(true);
        this->sunIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->sunIcon_->show();
    }
    if(model->weather->getCurrentCloudiness() > 0.5) {
        QPixmap cloud(":/images/cloud.png");
        this->cloudIcon_->setPixmap(cloud);
        this->cloudIcon_->setScaledContents(true);
        this->cloudIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->cloudIcon_->show();
    }
    if(model->weather->getCurrentRain() > 0.5 and model->weather->getCurrentTemperature() < 0) {

        // Ilmatieteen laitoksen mukaan vähän sadetta 0.3 - 0.9, sadetta 1 - 4.4 ja runsasta >4.5

        QPixmap rain(":/images/rain.png");
        this->rainIcon_->setPixmap(rain);
        this->rainIcon_->setScaledContents(true);
        this->rainIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->rainIcon_->show();
    }
    if(model->weather->getCurrentTemperature() <= 0) {
        QPixmap snow(":/images/snow.png");
        this->snowIcon_->setPixmap(snow);
        this->snowIcon_->setScaledContents(true);
        this->snowIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->snowIcon_->show();
    }
    if(model->weather->getCurrentWind() > 3) {
        QPixmap wind(":/images/wind.png");
        this->windIcon_->setPixmap(wind);
        this->windIcon_->setScaledContents(true);
        this->windIcon_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        this->windIcon_->show();
    }

}
