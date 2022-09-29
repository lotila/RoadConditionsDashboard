#include "mainwindow.h"

#include "model.h"

#include <QtWidgets>

#include <string>

MainWindow::MainWindow(Model* model, QWidget* parent) :
    QWidget(parent), model(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    this->view = new QLabel(this->model->weather->avgWind({0, 0}, {1, 2}).c_str());
    this->view->setAlignment(Qt::AlignCenter);
    this->view->setStyleSheet("font: 20pt;");

    layout->addWidget(this->view);

    this->otherButton = new QPushButton("Update", this);
    layout->addWidget(this->otherButton);

    this->button = new QPushButton("Quit", this);
    layout->addWidget(this->button);

    setLayout(layout);


    connect(this->otherButton, SIGNAL(clicked()), this, SIGNAL(itemClicked()));
    connect(button, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void MainWindow::updateItem()
{
    this->view->setText(this->model->weather->avgWind({0, 0}, {1, 2}).c_str());
}
