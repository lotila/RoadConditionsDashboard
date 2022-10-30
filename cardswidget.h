#ifndef CARDSWIDGET_H
#define CARDSWIDGET_H
#include "ui_cardswidget.h"
#include "qlabel.h"
#include <QFrame>
namespace Ui {
class CardsWidget;
}

class CardsWidget : public QFrame
{
    Q_OBJECT

public:
    CardsWidget( QWidget *parent = nullptr); // Constructor for empty Card widget
    
    // Constructor for cardwidget with header text and current value val
    CardsWidget(QString header, int val, QWidget *parent = nullptr); 
    ~CardsWidget();
    void setNowCard(QString text, int val);
    void setPredictedCard(QString text);
    void setIcon(QPixmap pixMap);
private:
    Ui::CardsWidget cardUi;
    QLabel* nowLabel_;
    QLabel* nowValueLabel_;
    QLabel* predictedLabel_;
    QLabel* iconLabel_;
    QWidget* chartWidget_;
};

#endif // CARDSWIDGET_H
