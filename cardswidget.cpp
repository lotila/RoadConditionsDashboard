#include "cardswidget.h"
#include "ui_cardswidget.h"
#include "linechart.h"

#include <iostream>
#include <unordered_map>

const std::unordered_map<float, QString> FRICTIONVALUES = {
    {0, "Normal condition"},
    {1, "Very slippery"},
    {2, "Slippery"}
};

const std::unordered_map<float, QString> VISIBILITYVALUES = {
    {0, "Normal condition"},
    {1, "Very poor"},
    {2, "Poor"}
};

const std::unordered_map<float, QString> CONDITIONVALUES = {
    {0, "Condition could not be resolved"},
    {1, "Extremely poor condition"},
    {2, "Poor condition"},
    {3, "Normal condition"}
};

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
        this->nowValueLabel_->setText(FRICTIONVALUES.at(value)); //Set the value of the card
    }
    else if (this->text == "Roadcondition")
    {
        this->cardChart->newPlot("" ,model->roadCondition->getRoadcondition());
        value = model->roadCondition->getCurrentRoadcondition();
        this->nowValueLabel_->setText(CONDITIONVALUES.at(value));
    }
    else if (this->text == "Visibility")
    {
        this->cardChart->newPlot("" ,model->roadCondition->getVisibility());
        value = model->roadCondition->getCurrentVisibility();
        this->nowValueLabel_->setText(VISIBILITYVALUES.at(value));
    }

    this->nowLabel_->setText(this->text + " Now"); //Set header for the Now card
}

CardsWidget::~CardsWidget()
{

}
