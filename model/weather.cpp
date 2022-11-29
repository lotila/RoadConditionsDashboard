#include "weather.h"

#include "APIC/apiclient.h"
#include "parsers/fmiparser.h"
#include "timeStamp.h"
#include "util.h"

#include <iostream>
#include <stdlib.h>
#include <string>

namespace models
{

const std::unordered_map<Weather::WEATHER_PARAMETER, std::string> Weather::weatherParameterToString = {
    {Weather::WEATHER_PARAMETER::TEMPERATURE, "temperature"},
    {Weather::WEATHER_PARAMETER::WIND, "WindSpeedMS"},
    {Weather::WEATHER_PARAMETER::RAIN, "Precipitation1h"},
    {Weather::WEATHER_PARAMETER::CLOUDINESS, "TotalCloudCover"}
};

Weather::Weather()
{



    // test data

    wind.push_back({1,15});wind.push_back({2,15});
    rain.push_back({1,10});rain.push_back({2,10});
    temperature.push_back({1,5});temperature.push_back({2,5});
    cloudiness.push_back({1,4});cloudiness.push_back({2,4});
    this->averageTemp = 5;
    this->maxTemp = 5;
    this->minTemp = 5;
}

std::string Weather::createObservationsQuery(WEATHER_PARAMETER parameter, const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    return "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&"
           "request=getFeature&"
           "storedquery_id=fmi::observations::weather::timevaluepair&"
           "starttime=" + isoDateHoursFromNow(timeSlot.start) + "&"
           "endtime=" + isoDateHoursFromNow(timeSlot.end) + "&"
           "timestep=60&"
           "&bbox="
            + std::to_string(coord.lon - util::NEARINDEGREES) + ","
            + std::to_string(coord.lat - util::NEARINDEGREES) + ","
            + std::to_string(coord.lon + util::NEARINDEGREES) + ","
            + std::to_string(coord.lat + util::NEARINDEGREES) + "&"
           "latlon=" + std::to_string(coord.lat) + "," + std::to_string(coord.lon) + "&"
           "parameters=" + weatherParameterToString.at(parameter) + "&";
}

std::string Weather::createForecastQuery(WEATHER_PARAMETER parameter, const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    return "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&"
           "storedquery_id=ecmwf::forecast::surface::point::timevaluepair&"
           "starttime=" + isoDateHoursFromNow(timeSlot.start) + "&"
           "endtime=" + isoDateHoursFromNow(timeSlot.end) + "&"
           "timestep=60&"
           "latlon=" + std::to_string(coord.lat) + "," + std::to_string(coord.lon) + "&"
           "parameters=" + weatherParameterToString.at(parameter) + "&";
}


util::TimeSeries Weather::genericUpdate(WEATHER_PARAMETER parameter, const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    util::TimeSeries result;

    if (timeSlot.start == 0 && timeSlot.end == 0)
    {
        return {};
    }

    if (timeSlot.start < 0)
    {
        util::TimeSlot observationTimeSlot = {timeSlot.start, timeSlot.end < 0 ? timeSlot.end : -1};
        std::string query = createObservationsQuery(parameter, coord, observationTimeSlot);
        std::string apiResponse;
        APIClient::getFMIAPIData(&apiResponse, query);

        util::TimeSeries observations = FMIParser::temperatureObservation(apiResponse, coord, observationTimeSlot);
        result.insert(result.end(), observations.begin(), observations.end());
    }

    if (timeSlot.end > 0)
    {
        util::TimeSlot forecastTimeSlot = {timeSlot.start >= 0 ? timeSlot.start : 0, timeSlot.end};
        std::string query = createForecastQuery(parameter, coord, forecastTimeSlot);
        std::string apiResponse;
        APIClient::getFMIAPIData(&apiResponse, query);

        util::TimeSeries forecasts = FMIParser::temperatureForecast(apiResponse, forecastTimeSlot);
        result.insert(result.end(), forecasts.begin(), forecasts.end());
    }
    return result;
}

void Weather::updateTemperature(const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    this->temperature.clear();
    this->temperature = this->genericUpdate(WEATHER_PARAMETER::TEMPERATURE, coord, timeSlot);
}

util::TimeSeries Weather::getTemperature() const
{
    return this->temperature;
}

float Weather::getCurrentTemperature() const
{
    if (this->temperature.empty())
    {
        std::cerr << "No temperature data" << std::endl;
        return 0;
    }
    return this->temperature.front().value;
}

float Weather::getAverageTemp() const
{
    return this->averageTemp;
}
float Weather::getMaxTemp() const
{
    return this->maxTemp;
}
float Weather::getMinTemp() const
{
    return this->minTemp;
}


void Weather::updateWind(const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    this->wind.clear();
    this->wind = this->genericUpdate(WEATHER_PARAMETER::WIND, coord, timeSlot);
}

util::TimeSeries Weather::getWind() const
{
    return this->wind;
}
float Weather::getCurrentWind() const
{
    if (this->wind.empty())
    {
         std::cerr << "No wind data" << std::endl;
         return 0;
    }
    return this->wind.front().value;
}

void Weather::updateRain(const util::Coord &coord, const util::TimeSlot& timeSlot)
{
    this->rain.clear();
    this->rain = this->genericUpdate(WEATHER_PARAMETER::RAIN, coord, timeSlot);
}

util::TimeSeries Weather::getRain() const
{
    return this->rain;
}

float Weather::getCurrentRain() const
{
    if (this->rain.empty())
    {
         std::cerr << "No rain data" << std::endl;
         return 0;
    }
    return this->rain.front().value;
}

void Weather::updateCloudiness(const util::Coord &coord, const util::TimeSlot& timeSlot)
{
    this->cloudiness.clear();
    this->cloudiness = this->genericUpdate(WEATHER_PARAMETER::CLOUDINESS, coord, timeSlot);
}

util::TimeSeries Weather::getCloudiness() const
{
    return this->cloudiness;
}

float Weather::getCurrentCloudiness() const
{
    if (this->cloudiness.empty())
    {
        std::cerr << "No Cloudiness data" << std::endl;
        return 0;
    }
    return this->cloudiness.front().value;
}


}
