#ifndef ROADCONDITION_H
#define ROADCONDITION_H

#include <string>

namespace models
{

class RoadCondition
{
public:
    RoadCondition();
    void updateRoadCondition();

    enum class CONDITION_TYPE
    {
        FORECAST,
        OBSERVATION
    };

private:
    int id;

    CONDITION_TYPE type = CONDITION_TYPE::FORECAST;

    std::string precipitation;
    std::string visibility;
    std::string friction;
    std::string overallRoadCondition;

    bool slippery;
    bool freezingRain;
    bool daylight;

    struct {
        std::string air;
        std::string road;
    } temperature;

    struct {
        float speed;
        int direction;
    } wind;
};

}

#endif
