#include "fmiparser.h"

#include "tinyxml/tinyxml2.h"

#include <limits>
#include <sstream>
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

std::vector<util::TimeValuePair> FMIParser::temperatureForecast(const std::string &input, const util::TimeSlot &timeSlot)
{
    tinyxml2::XMLDocument document(false);
    document.Parse(input.c_str());

    tinyxml2::XMLElement* timeSeries = document.RootElement()
        ->FirstChildElement("wfs:member")
        ->FirstChildElement("omso:PointTimeSeriesObservation")
        ->FirstChildElement("om:result")
        ->FirstChildElement("wml2:MeasurementTimeseries");

    tinyxml2::XMLElement* point = timeSeries->FirstChildElement("wml2:point");

    util::TimeSeries result;

    int timeComponent = timeSlot.start;
    while(point != nullptr)
    {
        std::string valueAsText = point
                ->FirstChildElement("wml2:MeasurementTVP")
                ->FirstChildElement("wml2:value")->GetText();
        result.push_back({timeComponent, std::stof(valueAsText)});

        ++timeComponent;
        point = point->NextSiblingElement("wml2:point");
    }

    return result;
}

util::TimeSeries FMIParser::temperatureObservation(const std::string &input, const util::Coord &coord, const util::TimeSlot &timeSlot)
{
    tinyxml2::XMLDocument document(false);
    document.Parse(input.c_str());

    tinyxml2::XMLElement* member = document.RootElement()->FirstChildElement("wfs:member");
    tinyxml2::XMLElement* closestMember = member;  // may be null
    float minimumDistance = std::numeric_limits<float>::max();

    while (member != nullptr)
    {
        std::string coordinatesAsText = member
                ->FirstChildElement("omso:PointTimeSeriesObservation")
                ->FirstChildElement("om:featureOfInterest")
                ->FirstChildElement("sams:SF_SpatialSamplingFeature")
                ->FirstChildElement("sams:shape")
                ->FirstChildElement("gml:Point")
                ->FirstChildElement("gml:pos")->GetText();


        std::stringstream ss(coordinatesAsText);
        std::string lat;
        std::string lon;
        getline(ss, lat, ' ');
        getline(ss, lon, ' ');


        float distance = coord.distance(util::Coord(std::stof(lat), std::stof(lon)));
        if (distance < minimumDistance)
        {
            closestMember = member;
        }

        member = member->NextSiblingElement("wfs::member");
    }

    if (closestMember == nullptr)
    {
        return {};
    }

    tinyxml2::XMLElement* timeSeries = closestMember
        ->FirstChildElement("omso:PointTimeSeriesObservation")
        ->FirstChildElement("om:result")
        ->FirstChildElement("wml2:MeasurementTimeseries");

    tinyxml2::XMLElement* point = timeSeries->FirstChildElement("wml2:point");

    util::TimeSeries result;

    int timeComponent = timeSlot.start;
    while(point != nullptr)
    {
        std::string valueAsText = point
                ->FirstChildElement("wml2:MeasurementTVP")
                ->FirstChildElement("wml2:value")->GetText();
        result.push_back({timeComponent, std::stof(valueAsText)});

        ++timeComponent;
        point = point->NextSiblingElement("wml2:point");
    }

    return result;
}
