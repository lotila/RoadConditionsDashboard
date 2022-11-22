#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include "ui_searchBar.h"

#include <QFrame>

namespace Ui {
class SearchBar;
}

class SearchBar: public QFrame
{
    Q_OBJECT

public:
    SearchBar(QWidget *parnet = nullptr); // Normal constructor
    SearchBar(QString placeName, QWidget *parent = nullptr); // Constructor when loading from saved preferences (needed?)

    QString getText();

private:
    Ui::SearchBarWidget searchBarUi;

    QString text_;

};


#endif // SEARCHBAR_H
