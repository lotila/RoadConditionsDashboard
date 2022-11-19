#include "weather.h"

#include "APIC/apiclient.h"
#include "parsers/fmiparser.h"
#include "util.h"

#include <iostream>
#include <stdlib.h>
#include <string>

namespace models
{

Weather::Weather()
{
    this->updateWeather();
}

const std::string &Weather::avgWind(const util::Coord& coord, const util::TimeSlot& time)
{
    return this->wind.current;
}

void Weather::updateWeather()
{
    std::string query = "service=WFS&version=2.0.0&request=getFeature&storedquery_id=livi::observations::road::default::timevaluepair&place=hervanta&parameters=windspeedms&";
    std::string apiResponse;
    APIClient::getFMIAPIData(&apiResponse, query);
    this->wind.current = FMIParser::wind(apiResponse);
}

}
