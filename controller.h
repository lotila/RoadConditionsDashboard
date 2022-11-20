#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mainwindow.h"
#include "model.h"
#include "util.h"
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Model* model, MainWindow* widget, QObject *parent = nullptr);
    void formTimeSlot(int); //Should this be private?
signals:
    void buttonClickFinished();
    void updateRoadDataModelFinished();

private slots:
    void buttonClicked();
    void updateRoadDataModel(int);
private:
    Model* model;
    MainWindow* widget;
    util::TimeSlot currentTimeSlot;
    //Here some kind of current time? So we wont update if time is same?
};

#endif
