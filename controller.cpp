#include "controller.h"
#include "mainwindow.h"
#include <iostream>
#include <QDebug>
#include "util.h"

Controller::Controller(Model *model, MainWindow* widget, QObject *parent)
    : QObject{parent}, model(model), widget(widget)
{
    connect(this->widget, SIGNAL(itemClicked()), this, SLOT(buttonClicked()));
    connect(this, SIGNAL(buttonClickFinished()), this->widget, SLOT(updateItem()));

    connect(this->widget, SIGNAL(updateTimeline(int)), this, SLOT(updateModelTimeline(int)));
    connect(this, SIGNAL(updateModelTimelineFinished()), this->widget, SLOT(updateView()));

    connect(this->widget, SIGNAL(updateCoordinates(util::Coord)), this, SLOT(updateModelCoordinates(util::Coord)));
    connect(this, SIGNAL(updateModelCoordinatesFinished()), this->widget, SLOT(updateView()));

    formTimeSlot(2); //Set the default time to 2h+
}

void Controller::buttonClicked()
{
    emit this->buttonClickFinished();
}

void Controller::updateModelTimeline(int timeValue)
{
    if(timeValue == 0) return; //Dont update model if time is set to 0
//Reciewed time:
    std::cout << "current time is " <<  timeValue << std::endl;
    //NOTE: Currently the timeValue is given either negatice or positive
    //So Past and Forecast data will only be requested separately
    formTimeSlot(timeValue);
    std::cout << "going to update model" << std::endl;
//TODO: Add the Model Updatefunctions here when they are ready or defined:
    //this->model->weather->updateWind({0, 0}, {1, 2}); // test location
    util::Coord currentPlace = this->widget->getCoordinates();
    updateModel(currentPlace, this->currentTimeSlot);

//Send signal when update done:
    emit this->updateModelTimelineFinished();
}

//Function for updating the model coordinates
void Controller::updateModelCoordinates(util::Coord newCoord)
{
    std::cout << "current COORD is " <<  newCoord.lat << " " << newCoord.lon << std::endl;
    //TODO: UPDate the model coordinates:
    updateModel(newCoord, this->currentTimeSlot);
    emit this->updateModelCoordinatesFinished();
}

void Controller::formTimeSlot(int time)
{
    if(time < 0)
    {
        this->currentTimeSlot = {time,0};
    }
    else
    {
        this->currentTimeSlot = {0,time};
    }
}

void Controller::updateWeatherIcon()
{

}

void Controller::updateModel(util::Coord currentPlace, util::TimeSlot currentTimeSlot)
{
    //weather
    this->model->weather->updateWind(currentPlace, currentTimeSlot);
    this->model->weather->updateCloudiness(currentPlace, currentTimeSlot);
    this->model->weather->updateRain(currentPlace, currentTimeSlot);
    this->model->weather->updateTemperature(currentPlace, currentTimeSlot);
    //road
    this->model->roadCondition->updateRoadCondition(currentPlace);
    //maintenance
    this->model->maintenance->updateMaintenance(currentPlace, currentTimeSlot);
    this->model->trafficMessages->updateTrafficMessageCount(currentPlace, currentTimeSlot);

}
