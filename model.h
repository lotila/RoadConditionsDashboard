#ifndef MODEL_H
#define MODEL_H

#include "model/weather.h"

struct Model
{
    Model()
    {
        this->weather = new models::Weather();
    }

    ~Model()
    {
        delete this->weather;
    }
    models::Weather* weather;
};

#endif
