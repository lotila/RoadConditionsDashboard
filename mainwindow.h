#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include "model.h"

#include <QtWidgets>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(Model* model, QWidget *parent = 0);
public slots:
    void updateItem();
signals:
    void itemClicked();
private:
    QLCDNumber *view;
    QPushButton *button;
    QPushButton *otherButton;

    Model* model;
};

#endif
