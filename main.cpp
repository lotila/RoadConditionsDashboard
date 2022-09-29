#include "controller.h"
#include "mainwindow.h"
#include "model.h"

#include <QApplication>
#include <QtGui>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    Model model;
    if (argc > 1 && std::strcmp(argv[1], "--no-gui") == 0)
    {
        std::cout << "wind is blowing like: "
                  << model.weather->avgWind({0, 0}, {1, 2})
                  << std::endl;
        return EXIT_SUCCESS;
    }
    else
    {
        QApplication app(argc, argv);
        MainWindow window = MainWindow(&model);
        Controller controller(&model, &window);
        window.show();
        return app.exec();
    }
}
