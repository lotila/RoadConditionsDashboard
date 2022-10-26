#include "fmiparser.h"

#include "tinyxml/tinyxml2.h"

#include <string>
#include <iostream>

std::string FMIParser::wind(const std::string& input)
{
    tinyxml2::XMLDocument document(false);
    document.Parse(input.c_str());

    std::string timeStamp = document.RootElement()
        ->Attribute("timeStamp");
    std::string dataAsText = document.RootElement()
        ->FirstChildElement("wfs:member")
        ->FirstChildElement("omso:PointTimeSeriesObservation")
        ->FirstChildElement("om:result")
        ->FirstChildElement("wml2:MeasurementTimeseries")
        ->LastChildElement("wml2:point")
        ->FirstChildElement("wml2:MeasurementTVP")
        ->FirstChildElement("wml2:value")->GetText();
    return "kello " + timeStamp + " tuuloo " + dataAsText + " m/s";
}
