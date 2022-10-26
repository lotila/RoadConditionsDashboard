#include "digitraffigparser.h"

#include "lib/json.hpp"

std::string digitraffigParser::example(const std::string &input)
{
    nlohmann::json data = nlohmann::json::parse(input);
    return data.at("features").at(0).at("properties").dump(2);
}
