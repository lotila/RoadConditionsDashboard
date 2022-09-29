#ifndef WEATHER_H
#define WEATHER_H

#include "util.h"

namespace models
{

class Weather
{
public:
    Weather();
    float avgWind(const util::Coord& coord, const util::TimeSlot& time);
    void updateWeather();

private:
    float wind = 4.5;
};

}

#endif
