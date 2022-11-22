#include "searchbar.h"
#include "ui_searchBar.h"

#include <iostream>

SearchBar::SearchBar(QWidget *parent) :
    QFrame(parent)
{
    Ui::SearchBarWidget searchBarUi;
    searchBarUi.setupUi(this);
}

// When retrieving saved settings
SearchBar::SearchBar(QString placeName, QWidget *parent) :
    QFrame(parent)
{
    Ui::SearchBarWidget searchBarUi;
    searchBarUi.setupUi(this);

    this->text_ = placeName;
    searchBarUi.searchBar->setText(this->text_);
}

QString SearchBar::getText()
{
    return this->text_;
}
