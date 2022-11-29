#include "cardswidget.h"
#include "ui_cardswidget.h"
#include <iostream>
#include "linechart.h"

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

CardsWidget::CardsWidget(QString text, Model* m, QWidget *parent) :
    QFrame(parent), model(m), text(text)
{
    Ui::CardsWidget cardUi; //Initializing UI from ui_mainwindow.h
    cardUi.setupUi(this);
    //Initializing card elements
    this->nowLabel_ = cardUi.labelNow;
    this->nowValueLabel_ = cardUi.labelNowValue;
    this->predictedLabel_ = cardUi.labelPredicted;
    this->iconLabel_ = cardUi.iconLabel;
    this->chartWidget_ = cardUi.emilWidget;

    setNowCard();

    setPredictedCard();
    updateCardInfo();
}

void CardsWidget::setNowCard()
{

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
void CardsWidget::setPredictedCard()
{
    this->predictedLabel_->setText(this->text+" Predicted");
    this->cardChart = new LineChart("", chartWidget_);
}

void CardsWidget::updateCardInfo()
{
    float value = 0;
    this->cardChart->deletePlots();
    if (this->text == "Friction")
    {
        this->cardChart->newPlot("" ,model->roadCondition->getFriction());
        value = model->roadCondition->getCurrentFriction();
    }
    else if (this->text == "Roadcondition")
    {
        this->cardChart->newPlot("" ,model->roadCondition->getRoadcondition());
        value = model->roadCondition->getCurrentRoadcondition();
    }
    else if (this->text == "Visibility")
    {
        this->cardChart->newPlot("" ,model->roadCondition->getVisibility());
        value = model->roadCondition->getCurrentVisibility();
    }

    QString StringValue = QString::number(value); //Change the val to Qstring
    this->nowLabel_->setText(this->text + " Now"); //Set header for the Now card
    this->nowValueLabel_->setText(StringValue + "%"); //Set the value of the card
}

CardsWidget::~CardsWidget()
{

}
