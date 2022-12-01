#include "roadcondition.h"

#include "APIC/apiclient.h"
#include "parsers/digitraffigparser.h"

#include <iostream>

namespace models
{

RoadCondition::RoadCondition()
{

    // test data
    /*this->friction.push_back({1,1}); this->friction.push_back({2,1});
    this->roadcondition.push_back({1,18}); this->roadcondition.push_back({2,18});
    this->visibility.push_back({1,60}); this->visibility.push_back({2,60});*/
}

void RoadCondition::updateRoadCondition(const util::Coord &coord)
{
    // Tällä samalla vois hakea myös friction ja visibility

    std::string path = "v3/data/road-conditions/";
    std::string query =   std::to_string(coord.lon - util::NEARINDEGREES) + "/"
                        + std::to_string(coord.lat - util::NEARINDEGREES) + "/"
                        + std::to_string(coord.lon + util::NEARINDEGREES) + "/"
                        + std::to_string(coord.lat + util::NEARINDEGREES) + "/";

    std::string apiResponse;
    APIClient::getDigitrafficAPIData(&apiResponse, path, query);

    std::vector<util::TimeValuePair> roadConditionVals = digitraffigParser::parseRoadCondition(apiResponse);
    std::vector<util::TimeValuePair> frictionVals = digitraffigParser::parseRoadFriction(apiResponse);
    std::vector<util::TimeValuePair> visibilityVals = digitraffigParser::parseRoadVisibility(apiResponse);

    this->roadcondition = roadConditionVals;
    this->friction = frictionVals;
    this->visibility = visibilityVals;
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
