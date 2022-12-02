#include "controller.h"
#include "mainwindow.h"
#include "model.h"
#include "APIC/apiclient.h"

#include <QApplication>
#include <QtGui>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    APIClient::INIT_STATUS status = APIClient::initializeAPIClient();
    if (status != APIClient::INIT_STATUS::OK)
    {
        return EXIT_FAILURE;
    }
    Model model;
    QApplication app(argc, argv);
    MainWindow window = MainWindow(&model);
    Controller controller(&model, &window);
    window.show();
    int appStatus = app.exec();
    APIClient::destructAPIClient();
    return appStatus;
}
