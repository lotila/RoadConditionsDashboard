#include "mainwindow.h"

#include "model.h"

#include <QtWidgets>

#include <string>

MainWindow::MainWindow(Model* model, QWidget* parent) :
    QWidget(parent), model(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->view = new QLCDNumber(this);
    layout->addWidget(view);

    this->otherButton = new QPushButton("Update", this);
    layout->addWidget(this->otherButton);

    this->button = new QPushButton("Quit", this);
    layout->addWidget(this->button);

    setLayout(layout);

    this->view->display(double(this->model->weather->avgWind({0, 0}, {1, 2})));

    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(button, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void MainWindow::updateItem()
{
    double newValue = this->model->weather->avgWind({0, 0}, {1, 2});
    this->view->display(newValue);
}
