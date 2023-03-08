#include "mainwindow.h"
#include "model.h"
#include "cardswidget.h"
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
    this->ui.searchBar->setPlaceholderText("Search by coordinates. Separate "
                                           "latitude and longitude with comma."
                                           "For example, Tampere is '61.4981,23.7608'");
//RoadFrame:
    //Initializing the Buttons for road Preview:
    this->nextButton_ = ui.nextButton_road;
    this->prevButton_ = ui.prevButton_road;

    //Connecting the prev and next buttons with updatefunction:
    connect(nextButton_, SIGNAL(clicked()), SLOT(updateCard()));
    connect(prevButton_, SIGNAL(clicked()), this, SLOT(updateCard()));

    //Initializing cards:
    this->cards_ = {"Friction", "Roadcondition", "Visibility",}; //Currently this is the DATA tells the card headers and amount of cards to be created
    this->index_ = 0; //Setting the current card(index 0)
    this->cardcontainer_ = {};
    createCards();



//Weather mainWindow Frame:
    updateMainWindowWeather();
    this->cardStackWidget->setCurrentWidget(cardcontainer_.at(index_));
    connect(ui.otherButton, SIGNAL(clicked()), this, SLOT(sendUpdateRequestForCoordinates()));
    setMainWindowWeatherIcons();


//RoadPage:
    this->roadDataChart = new LineChart("", ui.roadChartWidget);
    updateRoadDataChart();
    connect(this->ui.trafficMsgCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.cloudinessCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.rainCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.temperatureCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.frictionCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.windCheckboxRoadPage, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));

//Connections for Buttons in GUI:

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

//Here we should draw the charts again
void MainWindow::updateView()
{
    updateMainWindowWeather();
    updateCardInfo();
    updateRoadDataChart();
    updateWeatherData();
    updateWeatherIcons();

    // Update maintenance data piechart
    delete this->pieChart;
    this->pieChart = new PieChart("", this->ui.maintananceWorkPie, this->model->maintenance->getMaintanance());
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

void MainWindow::updateMainWindowWeather()
    {
    ui.lcdNumber_7->display(model->weather->getCurrentTemperature());
    ui.lcdNumber_9->display(model->weather->getCurrentWind());
    ui.lcdNumber_8->display(model->weather->getCurrentRain());
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
    util::Coord newCoords = this->getCoordinates();
    emit this->updateCoordinates(newCoords);
}

void MainWindow::setWeatherIcons()
{
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

void MainWindow::setMainWindowWeatherIcons()
{
    if(model->weather->getCurrentCloudiness() < 0.6) {
        QPixmap sun(":/images/sun.png");
        ui.sunIcon_4->setPixmap(sun);
        ui.sunIcon_4->setScaledContents(true);
        ui.sunIcon_4->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui.sunIcon_4->show();
    }
    if(model->weather->getCurrentCloudiness() > 0.5) {
        QPixmap cloud(":/images/cloud.png");
        ui.cloudIcon_4->setPixmap(cloud);
        ui.cloudIcon_4->setScaledContents(true);
        ui.cloudIcon_4->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui.cloudIcon_4->show();
    }
    if(model->weather->getCurrentRain() > 0.5 and model->weather->getCurrentTemperature() < 0) {
        QPixmap rain(":/images/rain.png");
        ui.rainIcon_5->setPixmap(rain);
        ui.rainIcon_5->setScaledContents(true);
        ui.rainIcon_5->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui.rainIcon_5->show();
    }
    if(model->weather->getCurrentTemperature() <= 0) {
        QPixmap snow(":/images/snow.png");
        ui.snowIcon_4->setPixmap(snow);
        ui.snowIcon_4->setScaledContents(true);
        ui.snowIcon_4->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui.snowIcon_4->show();
    }
    if(model->weather->getCurrentWind() > 3) {
        QPixmap wind(":/images/wind.png");
        ui.windIcon_4->setPixmap(wind);
        ui.windIcon_4->setScaledContents(true);
        ui.windIcon_4->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui.windIcon_4->show();
    }
}

void MainWindow::updateWeatherIcons()
{
    if(model->weather->getCurrentCloudiness() < 0.6) {
        this->sunIcon_->show();
        ui.sunIcon_4->show();
    }
    else
    {
         this->sunIcon_->hide();
         ui.sunIcon_4->hide();
    }
    if(model->weather->getCurrentCloudiness() > 0.5) {
        ui.cloudIcon_4->show();
        this->cloudIcon_->show();
    }
    else
    {
        ui.cloudIcon_4->hide();
        this->cloudIcon_->hide();
    }
    if(model->weather->getCurrentRain() > 0.5 and model->weather->getCurrentTemperature() < 0) {
        ui.rainIcon_5->show();
         this->rainIcon_->show();
    }
    else
    {
        ui.rainIcon_5->hide();
         this->rainIcon_->hide();
    }
    if(model->weather->getCurrentTemperature() <= 0) {
        ui.snowIcon_4->show();
        this->snowIcon_->show();
    }
    else
    {
        ui.snowIcon_4->hide();
        this->snowIcon_->hide();
    }
    if(model->weather->getCurrentWind() > 3) {
        ui.windIcon_4->show();
        this->windIcon_->show();
    }
    else
    {
        ui.windIcon_4->hide();
        this->windIcon_->hide();
    }
}

util::Coord MainWindow::getCoordinates()
{
    std::string searchInput = this->ui.searchBar->toPlainText().toStdString();

    std::string latitude = searchInput.substr(0, searchInput.find(','));
    std::string longitude = searchInput.substr(1 + searchInput.find(','), searchInput.size());

    util::Coord result = {61.45152, 23.84765}; //Default value AlePub Potti
    try {
        result = util::Coord(std::stof(latitude), std::stof(longitude));
    }
    catch (...) {
        ui.searchBar->setPlaceholderText("Syntax error. Make sure to "
                                         "separate latitude and longitude with comma."
                                         " For example, Tampere is '61.4981,23.7608'");
    }
    return result;
}
