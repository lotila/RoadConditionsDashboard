#ifndef FMIPARSER_H
#define FMIPARSER_H

#include "util.h"

#include <string>
#include <vector>

namespace FMIParser
{

std::string wind(const std::string& input);

std::vector<util::TimeValuePair> temperatureForecast(const std::string& input, const util::TimeSlot& timeSlot);
util::TimeSeries temperatureObservation(const std::string& input, const util::Coord& coord, const util::TimeSlot& timeSlot);

}


#endif
