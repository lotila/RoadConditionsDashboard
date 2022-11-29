#ifndef ROADCONDITION_H
#define ROADCONDITION_H

#include <string>
#include "util.h"
namespace models
{

class RoadCondition
{
public:
    RoadCondition();
    void updateRoadCondition();


    util::TimeSeries getFriction() const;
    float getCurrentFriction() const;
    util::TimeSeries getRoadcondition() const;
    float getCurrentRoadcondition() const;
    util::TimeSeries getVisibility() const;
    float getCurrentVisibility() const;


private:

    util::TimeSeries friction;


    util::TimeSeries roadcondition;

    util::TimeSeries visibility;

};

}

#endif
