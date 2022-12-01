#include "util.h"

#include <locale>
#include <sstream>

namespace util
{

Coord::Coord(float lat, float lon)
{
    if (lat > 90 || lat < -90)
    {
        throw std::invalid_argument("Invalid latitude value");
    }
    if (lon < -180 || lon > 180)
    {
        throw std::invalid_argument("Invalid longitude value");
    }

    this->lat = lat;
    this->lon = lon;
}

float Coord::distance(const Coord &other) const
{
    float latDistance = this->lat - other.lat;
    float lonDistance = this->lon - other.lon;
    return std::sqrt(std::pow(latDistance, 2) + std::pow(lonDistance, 2));
}

std::string toString(float number)
{
    std::ostringstream stream;
    stream.imbue(std::locale::classic());
    stream << number;
    return std::string(stream.str());
}

}
