#ifndef WEATHER_H
#define WEATHER_H

#include "util.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace models
{

class Weather
{
public:
    Weather();
    void updateTemperature(const util::Coord& coord, const util::TimeSlot& timeSlot);
    util::TimeSeries getTemperature();

    void updateWind(const util::Coord& coord, const util::TimeSlot& timeSlot);
    util::TimeSeries getWind();

    void updateRain(const util::Coord& coord, const util::TimeSlot& timeslot);
    util::TimeSeries getRain();

    void updateCloudiness(const util::Coord& coord, const util::TimeSlot& timeslot);
    util::TimeSeries getCloudiness();

    // old exmples
    const std::string& avgWind(const util::Coord& coord, const util::TimeSlot& time);
    void updateWeather();

private:


    enum class WEATHER_PARAMETER
    {
        TEMPERATURE,
        WIND,
        RAIN,
        CLOUDINESS
    };

    util::TimeSeries genericUpdate(WEATHER_PARAMETER parameter, const util::Coord& coord, const util::TimeSlot& timeSlot);

    static const std::unordered_map<WEATHER_PARAMETER, std::string> weatherParameterToString;

    static std::string createObservationsQuery(WEATHER_PARAMETER parameter, const util::Coord& coord, const util::TimeSlot& timeSlot);
    static std::string createForecastQuery(WEATHER_PARAMETER parameter, const util::Coord& coord, const util::TimeSlot& timeSlot);

    std::string currentWind;

    util::TimeSeries temperature;
    util::TimeSeries wind;
    util::TimeSeries rain;
    util::TimeSeries cloudiness;
};

}

#endif
