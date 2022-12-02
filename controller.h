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

    void updateWeatherIcon();
    void updateModel(util::Coord, util::TimeSlot);

signals:
    void buttonClickFinished();
    void updateModelTimelineFinished();
    void updateModelCoordinatesFinished();

private slots:
    void buttonClicked();
    void updateModelTimeline(int);
    void updateModelCoordinates(util::Coord);
private:
    Model* model;
    MainWindow* widget;
    util::TimeSlot currentTimeSlot;
};

#endif
