#ifndef DIGITRAFFIGPARSER_H
#define DIGITRAFFIGPARSER_H

#include "util.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_map>

namespace digitraffigParser
{

std::string example(const std::string& input);

std::vector<util::TimeValuePair> parseTrafficMessageCount(
        const std::string& input,
        const util::Coord& coord,
        const util::TimeSlot& timeSlot);

std::vector<util::TimeValuePair> parseRoadCondition(const std::string& input);
std::vector<util::TimeValuePair> parseRoadFriction(const std::string& input);
std::vector<util::TimeValuePair> parseRoadVisibility(const std::string& input);

std::unordered_map<std::string,int> parseMaintenances(const std::string& input);
}


#endif
