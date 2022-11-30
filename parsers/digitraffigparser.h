#ifndef DIGITRAFFIGPARSER_H
#define DIGITRAFFIGPARSER_H

#include "util.h"

#include <string>
#include <vector>

namespace digitraffigParser
{

std::string example(const std::string& input);

std::vector<util::TimeValuePair> parseTrafficMessageCount(
        const std::string& input,
        const util::Coord& coord,
        const util::TimeSlot& timeSlot);


std::vector<util::TimeValuePair> parseRoadCondition(const std::string& input);
}


#endif
