#ifndef UTIL_H
#define UTIL_H

namespace util {

struct Coord
{
    int lat;
    int lon;
};

struct TimeSlot
{
    // TODO: Think the name of this type
    int start;
    int end;
};

}

#endif
