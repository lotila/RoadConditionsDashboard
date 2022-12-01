#include "digitraffigparser.h"

#include "util.h"

#include "lib/json.hpp"
#include "lib/timeStamp.h"

#include <iostream>
#include <algorithm>
#include <string>

bool listIsNearToPoint(const nlohmann::json& list, const util::Coord& point)
{
    float distanceToStart = point.distance(
                {
                    list.at(0).at(1),
                    list.at(0).at(0)
                });

    nlohmann::json lastItem = list.at(list.size() - 1);

    float distanceToEnd = point.distance(
                {
                    lastItem.at(1),
                    lastItem.at(0)
                });


    return  (distanceToStart < util::NEARINDEGREES || distanceToEnd <  util::NEARINDEGREES);
}

/**
 * @param timeAndDuration see https://tie.digitraffic.fi/swagger/ and TimeAndDurationV1 component
 */
bool announcementIsValidInTimeSlot(const nlohmann::json& timeAndDuration, const util::TimeSlot& timeSlot)
{
    if (timeAndDuration.contains("endTime"))
    {
        std::string endTime = timeAndDuration.at("endTime").get<std::string>();
        if (endTime < isoDateHoursFromNow(timeSlot.start))
        {
            return false;
        }
    }
    if (timeAndDuration.at("startTime").get<std::string>() > isoDateHoursFromNow(timeSlot.end))
    {
        return false;
    }
    return true;
}



std::string digitraffigParser::example(const std::string &input)
{
    nlohmann::json data = nlohmann::json::parse(input);
    return data.at("features").at(0).at("properties").dump(2);
}

std::vector<util::TimeValuePair> digitraffigParser::parseTrafficMessageCount(
        const std::string &input,
        const util::Coord &coord,
        const util::TimeSlot &timeSlot)
{
    nlohmann::json data = nlohmann::json::parse(input);

    std::vector<util::TimeValuePair> result;
    for (int hoursBeforePresent = timeSlot.start; hoursBeforePresent < timeSlot.end; ++hoursBeforePresent)
    {
        int count = 0;
        for  (const nlohmann::json& j : data.at("features"))
        {
            if (!announcementIsValidInTimeSlot(j.at("properties").at("announcements").at(0).at("timeAndDuration"),
                                               {hoursBeforePresent, hoursBeforePresent + 1}))
            {
                continue;
            }

            if (j.at("geometry").is_null())
            {
                // Not sure if we should not handle these at all.
                // After all these are spec violations from digitraffic
                continue;
            }
            nlohmann::json geometry = j.at("geometry");
            std::string geometryType = geometry.at("type");

            if (geometryType == "Point")
            {
                float distance = coord.distance(
                            {
                                // GeoJSON has those wrong way
                                geometry.at("coordinates").at(1),
                                geometry.at("coordinates").at(0)
                            });

                if (distance < util::NEARINDEGREES)
                {
                    ++count;
                }
            }
            else if (geometryType == "LineString")
            {
                if (listIsNearToPoint(geometry.at("coordinates"), coord))
                {
                    ++count;
                }
            }
            else if (geometryType == "MultiLineString")
            {
                for(const nlohmann::json& lineString : geometry.at("coordinates"))
                {
                    if (listIsNearToPoint(lineString, coord))
                    {
                        ++count;
                        break;
                    }
                }
            }
            else
            {
                std::cerr << geometryType << " is not handled because they are minority."<< std::endl;
            }
        }
        result.push_back({hoursBeforePresent, float(count)});
    }

    return result;
}

util::TimeValuePair getData(nlohmann::json obj, char flag = 'r')
{
    if(flag == 'f') { // friction
        float condition = 0;
        std::string timeString = obj.at("forecastName");
        util::TimeValuePair t;
        t.time = stoi(timeString.substr(0, timeString.back()-1));

        if(obj.find("forecastConditionReason") == obj.end() or
            obj.at("forecastConditionReason").find("frictionCondition") == obj.at("forecastConditionReason").end()) {

            t.value = condition;
            return t;
        }

        if(obj.at("forecastConditionReason").at("frictionCondition") == "SLIPPERY") condition = 1;
        if(obj.at("forecastConditionReason").at("frictionCondition") == "VERY_SLIPPERY") condition = 2;
        t.value = condition;
        return t;
    }
    else if(flag == 'v') { // visibility
        float condition = 0;
        std::string timeString = obj.at("forecastName");
        util::TimeValuePair t;
        t.time = stoi(timeString.substr(0, timeString.back()-1));

        if(obj.find("forecastConditionReason") == obj.end() or
            obj.at("forecastConditionReason").find("visibilityCondition") == obj.at("forecastConditionReason").end()) {

            t.value = condition;
            return t;
        }

        if(obj.at("forecastConditionReason").at("visibilityCondition") == "FAIRLY_POOR") condition = 1;
        if(obj.at("forecastConditionReason").at("visibilityCondition") == "POOR") condition = 2;
        t.value = condition;
        return t;
    }
    else {
        float condition;
        //RoadCOndition maps to 4 differest values:
        if(obj.at("overallRoadCondition") == "NORMAL_CONDITION") condition = 3;
        if(obj.at("overallRoadCondition") == "POOR_CONDITION") condition = 2;
        if(obj.at("overallRoadCondition") == "EXTREMELY_POOR_CONDITION") condition = 1;
        if(obj.at("overallRoadCondition") == "CODITION_COULD_NOT_BE_RESOLVED") condition = 0;
        std::string timeString = obj.at("forecastName");
        timeString.substr(0, timeString.back()-1); //removing th h from the end of the string
        int timevalue = stoi(timeString);
        util::TimeValuePair t;
        t.time = timevalue;
        t.value = condition;
        return t;
    }
}

std::vector<util::TimeValuePair> digitraffigParser::parseRoadCondition(const std::string &input)
{
    nlohmann::json data = nlohmann::json::parse(input);
    const nlohmann::json& recent_data = data.at("weatherData").back().at("roadConditions"); //get the most recent observation+forecast
    std::vector<util::TimeValuePair> result;

    for(const nlohmann::json& j : recent_data)
    {
        result.push_back(getData(j));
    }

    return result;
}

std::vector<util::TimeValuePair> digitraffigParser::parseRoadFriction(const std::string &input)
{
    nlohmann::json data = nlohmann::json::parse(input);
    const nlohmann::json& recent_data = data.at("weatherData").back().at("roadConditions"); //get the most recent observation+forecast
    std::vector<util::TimeValuePair> result;

    for(const nlohmann::json& j : recent_data)
    {
        result.push_back(getData(j, 'f'));
    }

    return result;
}

std::vector<util::TimeValuePair> digitraffigParser::parseRoadVisibility(const std::string &input)
{
    nlohmann::json data = nlohmann::json::parse(input);
    const nlohmann::json& recent_data = data.at("weatherData").back().at("roadConditions"); //get the most recent observation+forecast
    std::vector<util::TimeValuePair> result;

    for(const nlohmann::json& j : recent_data)
    {
        result.push_back(getData(j, 'v'));
    }

    return result;
}


std::unordered_map<std::string, int> digitraffigParser::parseMaintenances(const std::string &input)
{
    std::unordered_map<std::string, int>  result;

    nlohmann::json data = nlohmann::json::parse(input);
    for (const nlohmann::json& feature : data.at("features"))
    {
        for (const nlohmann::json& taskAsJson : feature.at("properties").at("tasks"))
        {
            std::string task = taskAsJson.get<std::string>();
            if (result.count(task) == 0)
            {
                result.insert({task, 0});
            }

            ++result.at(task);
        }
    }
    std::cout << data.at("features").size() << std::endl;

    return result;
}
