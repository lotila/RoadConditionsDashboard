#include "weather.h"

#include "util.h"

namespace models
{

Weather::Weather()
{
    // TODO
}

float Weather::avgWind(const util::Coord& coord, const util::TimeSlot& time)
{
    // TODO
    return this->wind;
}

void Weather::updateWeather()
{
    if (this->wind < 5)
    {
        this->wind = 42;
    }
    else
    {
        this->wind = 4.7;
    }
}

}
