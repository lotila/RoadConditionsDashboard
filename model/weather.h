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
    util::TimeSeries getTemperature() const;

    float getCurrentTemperature() const;
    float getAverageTemp() const;
    float getMaxTemp() const;
    float getMinTemp() const;


    void updateWind(const util::Coord& coord, const util::TimeSlot& timeSlot);
    util::TimeSeries getWind() const;
    float getCurrentWind() const;

    void updateRain(const util::Coord& coord, const util::TimeSlot& timeslot);
    util::TimeSeries getRain() const;
    float getCurrentRain() const;

    void updateCloudiness(const util::Coord& coord, const util::TimeSlot& timeslot);
    util::TimeSeries getCloudiness() const;
    float getCurrentCloudiness() const;

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

    std::string currentWindTest;

    util::TimeSeries temperature;
    util::TimeSeries wind;
    util::TimeSeries rain;
    util::TimeSeries cloudiness;

    float averageTemp;
    float maxTemp;
    float minTemp;


};

}

#endif
