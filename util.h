#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <stdexcept>
#include <vector>

namespace util {

const float NEARINDEGREES = 0.5;

class Coord
{
public:
    Coord(float lat, float lon);

    float distance(const Coord& other) const;

    float lat;
    float lon;
};

struct TimeSlot
{
    // hours from present
    // past is expressed as negative number
    int start;
    int end;
};

struct TimeValuePair
{
    int time;  // in hours from present
    float value;
};

using TimeSeries = std::vector<util::TimeValuePair>;

}

#endif
