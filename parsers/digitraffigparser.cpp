#include "digitraffigparser.h"

#include "util.h"

#include "lib/json.hpp"
#include "lib/timeStamp.h"

#include <iostream>

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


    return  (distanceToStart < util::NEAR || distanceToEnd <  util::NEAR);
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

                if (distance < util::NEAR)
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
