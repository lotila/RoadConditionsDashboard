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
    this->roadDataChart = new LineChart("", ui.roadChartWidget);
    updateRoadDataChart();

    connect(this->ui.trafficMsgCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.cloudinessCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.rainCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.temperatureCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.frictionCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));
    connect(this->ui.windCheckbox, SIGNAL(stateChanged(int)), this, SLOT(updateRoadDataChart()));


//Connections for ButBtons in GUI:


    //Initialize chart setting elements:
    this->timeLineSlider = this->ui.timeLineSlider;
    this->timeLineLabel = this->ui.timeLineSelected;
    this->timeLineSlider->setRange(-12, 12);

    this->timeLineLabel->setText("0");
    //update the Timeline position actively
    connect(this->timeLineSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTimeLineLabel(int)));
    //Update model when timeline is selected(handle is released):
    connect(this->timeLineSlider, SIGNAL(sliderReleased()), this, SLOT(sendUpdateRequestForRoadData()));

    connect(this->ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));
    connect(this->ui.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));
    connect(this->ui.checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(updateWeatherData()));


//WeatherPage:
    this->weatherChart = new LineChart("", ui.weatherChart);
    //Plotting the wanted data:
    updateWeatherData();


//Connections for Buttons in GUI:
    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(ui.moreButton_road, SIGNAL(clicked()), this, SLOT(switchToRoadPage()));
    connect(ui.moreButton_weather, SIGNAL(clicked()), this, SLOT(switchToWeatherPage()));
    connect(ui.homeButton_road, SIGNAL(clicked()), this, SLOT(switchToMainPage()));


// maintanance work
    this->pieChart = new PieChart("", this->ui.maintananceWorkPie, this->model->maintenance->getMaintanance());

}

void MainWindow::updateItem()
{
    this->view->setText(this->model->weather->avgWind({0, 0}, {1, 2}).c_str());
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
    if (ui.windCheckbox->isChecked()) this->roadDataChart->newPlot("wind" ,model->weather->getWind());
    if (ui.temperatureCheckbox->isChecked()) this->roadDataChart->newPlot("temperature" ,model->weather->getTemperature());
    if (ui.rainCheckbox->isChecked()) this->roadDataChart->newPlot("rain" ,model->weather->getRain());
    if (ui.cloudinessCheckbox->isChecked()) this->roadDataChart->newPlot("Cloudiness" ,model->weather->getCloudiness());
    if (ui.trafficMsgCheckbox->isChecked()) this->roadDataChart->newPlot("traffic messages" ,model->trafficMessages->getTrafficMessageCount());
    if (ui.frictionCheckbox->isChecked()) this->roadDataChart->newPlot("friction" ,model->roadCondition->getFriction());

}


void MainWindow::updateWeatherChartData()
{
    this->weatherChart->deletePlots();
    if (ui.checkBox->isChecked()) this->weatherChart->newPlot("wind" ,model->weather->getWind());
    if (ui.checkBox_3->isChecked()) this->weatherChart->newPlot("temperature" ,model->weather->getTemperature());
    if (ui.checkBox_2->isChecked()) this->weatherChart->newPlot("rain" ,model->weather->getRain());
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
    QString timeLineText = QString::number(newValue);
    this->timeLineLabel->setText(timeLineText);
}

void MainWindow::sendUpdateRequestForRoadData()
{
    int timePos = this->timeLineSlider->sliderPosition();
    emit this->updateRoadData(timePos);
}

