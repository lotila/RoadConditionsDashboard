#include "cardswidget.h"
#include "ui_cardswidget.h"
#include <iostream>
#include "chart.h"

CardsWidget::CardsWidget(QWidget *parent) :
    QFrame(parent)
{
    Ui::CardsWidget cardUi; //Initializing UI from ui_mainwindow.h
    cardUi.setupUi(this);

    this->nowLabel_ = cardUi.labelNow;
    this->nowValueLabel_ = cardUi.labelNowValue;
    this->predictedLabel_ = cardUi.labelPredicted;
    this->iconLabel_ = cardUi.iconLabel;
    this->chartWidget_ = cardUi.emilWidget;
}

CardsWidget::CardsWidget(QString text, int val, QWidget *parent) :
    QFrame(parent)
{
    Ui::CardsWidget cardUi; //Initializing UI from ui_mainwindow.h
    cardUi.setupUi(this);
    //Initializing card elements
    this->nowLabel_ = cardUi.labelNow;
    this->nowValueLabel_ = cardUi.labelNowValue;
    this->predictedLabel_ = cardUi.labelPredicted;
    this->iconLabel_ = cardUi.iconLabel;
    this->chartWidget_ = cardUi.emilWidget;

    setNowCard(text, val);

    //Setting the predicted card:
    this->predictedLabel_->setText(text + "Predicted");
    setPredictedCard(text);
}

void CardsWidget::setNowCard(QString text, int val)
{
    QString value = QString::number(val); //Change the val to Qstring
    this->nowLabel_->setText(text + " Now"); //Set header for the Now card
    this->nowValueLabel_->setText(value + "%"); //Set the value of the card
   //Set the icon:
    QString img_name = text.toLower(); //Variablee name to lowercase(e.g. friction, visibility...
    img_name = img_name+".png"; //image name(e.g. friction.png, visibility.png...
    //Set icon based on the resource path parameter:
    QPixmap m(":/images/"+img_name); //get the image and make it a pixmap
    setIcon(m);
}
void CardsWidget::setIcon(QPixmap m)
{
    if(!m.isNull())
    {
        this->iconLabel_->setPixmap(m);
        this->iconLabel_->show();
        return;
    }

    this->iconLabel_->setText("Icon coming soon :)");
}
void CardsWidget::setPredictedCard(QString text)
{
    this->predictedLabel_->setText(text+" Predicted");
    chart* cardChart = new chart("esimerkki kaavio", chartWidget_);
   // cardChart->XaxisLabel("esim akseli X");
    //cardChart->YaxisLabel("esim akseli Y");

    std::vector<point2d> data = {point2d(1,2), point2d(2,3),point2d(3,1),point2d(4,0),point2d(5,2)};
    std::vector<point2d> data2 = {point2d(1,2), point2d(2,3),point2d(3,4),point2d(9,0),point2d(17,4)};

    cardChart->newPlot("esimerkki kuvaaja" ,data);
    cardChart->newPlot("esimerkki kuvaaja 2" ,data2);
}

CardsWidget::~CardsWidget()
{

}
