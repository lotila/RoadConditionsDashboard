#include "controller.h"
#include "mainwindow.h"

#include <QDebug>

Controller::Controller(Model *model, MainWindow* widget, QObject *parent)
    : QObject{parent}, model(model), widget(widget)
{
    connect(this->widget, SIGNAL(itemClicked()), this, SLOT(buttonClicked()));
    connect(this, SIGNAL(buttonClickFinished()), this->widget, SLOT(updateItem()));
}

void Controller::buttonClicked()
{
    this->model->weather->updateWeather();
    emit this->buttonClickFinished();
}
