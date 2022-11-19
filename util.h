#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <stdexcept>

namespace util {

const float NEAR = 0.5;  // in degrees

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

}

#endif
