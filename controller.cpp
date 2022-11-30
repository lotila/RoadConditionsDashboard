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

    connect(this->widget, SIGNAL(updateRoadData(int)), this, SLOT(updateRoadDataModel(int)));
    connect(this, SIGNAL(updateRoadDataModelFinished()), this->widget, SLOT(updateRoadDataChart()));
}

void Controller::buttonClicked()
{
    //this->model->weather->updateWind({0, 0}, {1, 2});// test location
    emit this->buttonClickFinished();
}
void Controller::updateRoadDataModel(int timeValue)
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

//Send signal when update done:
    emit this->updateRoadDataModelFinished();
}

void Controller::formTimeSlot(int time)
{
    if(time < 0)
    {
        this->currentTimeSlot = {time,0};
    }
    this->currentTimeSlot = {0,time};
}

void Controller::updateWeatherIcon()
{

}
