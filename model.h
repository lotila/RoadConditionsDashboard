#ifndef MODEL_H
#define MODEL_H

#include "model/weather.h"
#include "model/maintenance.h"
#include "model/roadcondition.h"
#include "model/trafficmessages.h"

struct Model
{
    Model()
    {
        this->weather = new models::Weather();
        this->maintenance = new models::Maintenance();
        this->roadCondition = new models::RoadCondition();
        this->trafficMessages = new models::TrafficMessages();
    }

    ~Model()
    {
        delete this->weather;
        delete this->maintenance;
        delete this->roadCondition;
        delete this->trafficMessages;
    }
    models::Weather* weather;
    models::Maintenance* maintenance;
    models::RoadCondition* roadCondition;
    models::TrafficMessages* trafficMessages;
};

#endif
