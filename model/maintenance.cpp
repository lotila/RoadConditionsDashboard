#include "maintenance.h"

#include "APIC/apiclient.h"
#include "parsers/digitraffigparser.h"

#include "timeStamp.h"

#include <charconv>
#include <iostream>
#include <string>
#include <unordered_map>

namespace models
{

Maintenance::Maintenance()
{
}

void Maintenance::updateMaintenance(const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    // There is no maintenance task forecasts
    if (timeSlot.start >= 0)
    {
        this->data = {};
        return;
    }

    int clampedTimeSlotEnd = timeSlot.end >= 0 ? 0 : timeSlot.end;

    if (timeSlot.start + clampedTimeSlotEnd < -24)
    {
        throw TooLongTimeSlot();
    }

    std::string apiResponse;

    std::string path = "maintenance/v1/tracking/routes";
    std::string query = "endFrom=" + isoDateHoursFromNow(timeSlot.start) + "&"
                        "endBefore=" + isoDateHoursFromNow(clampedTimeSlotEnd) + "&"
                        "xMin=" + util::toString(coord.lon - util::NEARINDEGREES) + "&"
                        "yMin=" + util::toString(coord.lat - util::NEARINDEGREES) + "&"
                        "xMax=" + util::toString(coord.lon + util::NEARINDEGREES) + "&"
                        "yMax=" + util::toString(coord.lat + util::NEARINDEGREES);


    APIClient::getDigitrafficAPIData(&apiResponse, path, query);
    this->data = digitraffigParser::parseMaintenances(apiResponse);
}

std::unordered_map<std::string, int> Maintenance::getMaintanance()
{
    //this->updateMaintenance();  // TODO: this is test, remove
    return this->data;
}

}
