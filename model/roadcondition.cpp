#include "roadcondition.h"

#include "APIC/apiclient.h"
#include "parsers/digitraffigparser.h"
#include "util.h"

#include <iostream>

namespace models
{

RoadCondition::RoadCondition()
{
    // Some initial data
    this->updateRoadCondition(util::Coord(61.45152, 23.84765)); // AlePub Potti
}

void RoadCondition::updateRoadCondition(const util::Coord &coord)
{
    std::string path = "v3/data/road-conditions/"
            + util::toString(coord.lon - util::NEARINDEGREES) + "/"
            + util::toString(coord.lat - util::NEARINDEGREES) + "/"
            + util::toString(coord.lon + util::NEARINDEGREES) + "/"
            + util::toString(coord.lat + util::NEARINDEGREES) + "/";

    std::string apiResponse;
    APIClient::getDigitrafficAPIData(&apiResponse, path, "");

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
