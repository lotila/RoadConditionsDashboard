#ifndef CARDSWIDGET_H
#define CARDSWIDGET_H
#include "ui_cardswidget.h"
#include "qlabel.h"
#include <QFrame>
#include "linechart.h"
#include "model.h"

namespace Ui {
class CardsWidget;
}

class CardsWidget : public QFrame
{
    Q_OBJECT

public:
    CardsWidget( QWidget *parent = nullptr); // Constructor for empty Card widget
    
    // Constructor for cardwidget with header text and current value val
    CardsWidget(QString header, Model* m, QWidget *parent = nullptr);
    ~CardsWidget();
    void setNowCard();
    void setPredictedCard();
    void setIcon(QPixmap pixMap);

    void updateCardInfo();
private:
    Ui::CardsWidget cardUi;
    QLabel* nowLabel_;
    QLabel* nowValueLabel_;
    QLabel* predictedLabel_;
    QLabel* iconLabel_;
    QWidget* chartWidget_;

    LineChart* cardChart;
    Model* model;

    QString text;


};

#endif // CARDSWIDGET_H
