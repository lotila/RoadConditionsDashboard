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
    if (argc > 1 && std::strcmp(argv[1], "--no-gui") == 0)
    {
        std::cout << "wind is blowing like: "
                  << model.weather->avgWind({0, 0}, {1, 2})
                  << std::endl;
        APIClient::destructAPIClient();
        return EXIT_SUCCESS;
    }
    else
    {
        QApplication app(argc, argv);
        MainWindow window = MainWindow(&model);
        Controller controller(&model, &window);
        window.show();
        int appStatus = app.exec();
        APIClient::destructAPIClient();
        return appStatus;
    }
}
