#ifndef WEATHER_H
#define WEATHER_H

#include "util.h"

#include <string>

namespace models
{

class Weather
{
public:
    Weather();
    const std::string& avgWind(const util::Coord& coord, const util::TimeSlot& time);
    void updateWeather();

private:

    std::string cloudiness;

    struct {
        std::string current;
        std::string forecast;
    } wind;

    struct {
        float current;
        float forecast;
    } temperature;
};

}

#endif
