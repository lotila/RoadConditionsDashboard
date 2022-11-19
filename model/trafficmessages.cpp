#include "trafficmessages.h"

#include "APIC/apiclient.h"
#include "parsers/digitraffigparser.h"

#include <iostream>
#include <string>

namespace models
{

TrafficMessages::TrafficMessages()
{

}

void TrafficMessages::updateTrafficMessageCount(const util::Coord& coord, const util::TimeSlot& timeSlot)
{
    // messages from future are not possible.
    if (timeSlot.start >= 0)
    {
        this->data = {};
        return;
    }

    std::string apiResponse;

    std::string path = "traffic-message/v1/messages";
    int inactiveHours = -1 * (timeSlot.start - 1);
    std::string query = "inactiveHours=" + std::to_string(inactiveHours)
            + "&includeAreaGeometry=false&situationType=TRAFFIC_ANNOUNCEMENT";

    APIClient::getDigitrafficAPIData(&apiResponse, path, query);
    this->data = digitraffigParser::parseTrafficMessageCount(apiResponse, coord, timeSlot);
}

std::vector<util::TimeValuePair> TrafficMessages::getTrafficMessageCount()
{
    return this->data;
}

}
