#ifndef TRAFFICMESSAGES_H
#define TRAFFICMESSAGES_H

#include "util.h"

#include <string>
#include <vector>

namespace models
{

class TrafficMessages
{
public:
    TrafficMessages();

    void updateTrafficMessageCount(const util::Coord& coord, const util::TimeSlot& timeSlot);

    std::vector<util::TimeValuePair> getTrafficMessageCount() const;

private:
    std::vector<util::TimeValuePair> data;
};

}

#endif
