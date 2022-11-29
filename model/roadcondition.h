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


    //int id;

    //CONDITION_TYPE type = CONDITION_TYPE::FORECAST;

    //std::string precipitation;
    //std::string visibility;
    //std::string friction;
    //std::string overallRoadCondition;

    //bool slippery;
    //bool freezingRain;
    //bool daylight;

    //struct {
    //    std::string air;
    //    std::string road;
    //} temperature;

    //struct {
    //    float speed;
    //    int direction;
    //} wind;
};

}

#endif
