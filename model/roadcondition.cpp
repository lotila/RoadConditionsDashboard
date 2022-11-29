#include "roadcondition.h"

namespace models
{

RoadCondition::RoadCondition()
{

    // test data
    this->friction.push_back({1,1}); this->friction.push_back({2,1});
    this->roadcondition.push_back({1,18}); this->roadcondition.push_back({2,18});
    this->visibility.push_back({1,60}); this->visibility.push_back({2,60});
}

void RoadCondition::updateRoadCondition()
{

}

util::TimeSeries RoadCondition::getFriction() const
{
    return this->friction;
}

float RoadCondition::getCurrentFriction() const
{
    return this->friction.front().value;
}
util::TimeSeries RoadCondition::getRoadcondition() const
{
    return this->roadcondition;
}
float RoadCondition::getCurrentRoadcondition() const
{
    return this->roadcondition.front().value;
}
util::TimeSeries RoadCondition::getVisibility() const
{
    return this->visibility;
}
float RoadCondition::getCurrentVisibility() const
{
    return this->visibility.front().value;
}

}
