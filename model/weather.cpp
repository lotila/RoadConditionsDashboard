#include "weather.h"

#include "APIC/apiclient.h"
#include "parsers/fmiparser.h"
#include "timeStamp.h"
#include "util.h"

#include <algorithm>  // min and max of timeseries
#include <cmath>  // NAN
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
    this->updateWind({60.2, 24.9}, {-12, 24});
    this->updateRain({60.2, 24.9}, {-12, 24});
    this->updateCloudiness({60.2, 24.9}, {-12, 24});
    this->updateTemperature({60.2, 24.9}, {-12, 24});
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
            + util::toString(coord.lon - util::NEARINDEGREES) + ","
            + util::toString(coord.lat - util::NEARINDEGREES) + ","
            + util::toString(coord.lon + util::NEARINDEGREES) + ","
            + util::toString(coord.lat + util::NEARINDEGREES) + "&"
           "parameters=" + weatherParameterToString.at(parameter) + "&";
}

std::string Weather::createForecastQuery(WEATHER_PARAMETER parameter, const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    return "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&"
           "storedquery_id=ecmwf::forecast::surface::point::timevaluepair&"
           "starttime=" + isoDateHoursFromNow(timeSlot.start) + "&"
           "endtime=" + isoDateHoursFromNow(timeSlot.end) + "&"
           "timestep=60&"
           "latlon=" + util::toString(coord.lat) + "," + util::toString(coord.lon) + "&"
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

void Weather::genericCurrentValueUpdate(WEATHER_PARAMETER parameter, const util::Coord &coord)
{

    util::TimeSlot forecastTimeSlot = {0, 1};
    std::string query = createForecastQuery(parameter, coord, forecastTimeSlot);
    std::string apiResponse;
    APIClient::getFMIAPIData(&apiResponse, query);

    util::TimeSeries result = FMIParser::temperatureForecast(apiResponse, forecastTimeSlot);
    this->currentValues.at(parameter) = result.front().value;
}

void Weather::updateTemperature(const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    this->temperature.clear();
    this->temperature = this->genericUpdate(WEATHER_PARAMETER::TEMPERATURE, coord, timeSlot);
    this->genericCurrentValueUpdate(WEATHER_PARAMETER::TEMPERATURE, coord);

    if (this->temperature.empty())
    {
        this->maxTemp = NAN;
        this->minTemp = NAN;
        this->averageTemp = NAN;
        return;
    }

    // Calculations
    util::TimeSeries::iterator result = std::max_element(this->temperature.begin(), this->temperature.end());
    this->maxTemp = result->value;

    result = std::min_element(this->temperature.begin(), this->temperature.end());
    this->minTemp = result->value;

    float sum = 0.0;
    for (const util::TimeValuePair& pair : this->temperature)
    {
        sum += pair.value;
    }
    this->averageTemp = sum / this->temperature.size();
}

util::TimeSeries Weather::getTemperature() const
{
    return this->temperature;
}

float Weather::getCurrentTemperature() const
{
    return this->currentValues.at(WEATHER_PARAMETER::TEMPERATURE);
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
    this->genericCurrentValueUpdate(WEATHER_PARAMETER::WIND, coord);
}

util::TimeSeries Weather::getWind() const
{
    return this->wind;
}

float Weather::getCurrentWind() const
{
    return this->currentValues.at(WEATHER_PARAMETER::WIND);
}

void Weather::updateRain(const util::Coord &coord, const util::TimeSlot& timeSlot)
{
    this->rain.clear();
    this->rain = this->genericUpdate(WEATHER_PARAMETER::RAIN, coord, timeSlot);
    this->genericCurrentValueUpdate(WEATHER_PARAMETER::RAIN, coord);
}

util::TimeSeries Weather::getRain() const
{
    return this->rain;
}

float Weather::getCurrentRain() const
{
    return this->currentValues.at(WEATHER_PARAMETER::RAIN);
}

void Weather::updateCloudiness(const util::Coord &coord, const util::TimeSlot& timeSlot)
{
    this->cloudiness.clear();
    this->cloudiness = this->genericUpdate(WEATHER_PARAMETER::CLOUDINESS, coord, timeSlot);
    this->genericCurrentValueUpdate(WEATHER_PARAMETER::CLOUDINESS, coord);
}

util::TimeSeries Weather::getCloudiness() const
{
    return this->cloudiness;
}

float Weather::getCurrentCloudiness() const
{
    return this->currentValues.at(WEATHER_PARAMETER::CLOUDINESS);
}


}
