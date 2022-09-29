#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mainwindow.h"
#include "model.h"

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Model* model, MainWindow* widget, QObject *parent = nullptr);

signals:
    void buttonClickFinished();

private slots:
    void buttonClicked();

private:
    Model* model;
    MainWindow* widget;
};

#endif
