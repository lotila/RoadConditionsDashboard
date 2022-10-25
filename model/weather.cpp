#include "weather.h"

#include "util.h"
#include "APIC/curlutil.h"

#include <curl/curl.h>
#include "tinyxml/tinyxml2.h"

#include <iostream>
#include <stdlib.h>
#include <string>

namespace models
{

Weather::Weather()
{
    // TODO
    this->updateWeather();
}

const std::string &Weather::avgWind(const util::Coord& coord, const util::TimeSlot& time)
{
    return this->wind.current;
}

void Weather::updateWeather()
{
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    curl_handle = curl_easy_init();
    if(curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=livi::observations::road::default::timevaluepair&place=hervanta&parameters=windspeedms&");
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "salamyhkainen_agentti");

        res = curl_easy_perform(curl_handle);

        if(res != CURLE_OK) {
            std::cerr << "fuck: " << curl_easy_strerror(res) << std::endl;
            this->wind.current = "virhes";
        } else {
            tinyxml2::XMLDocument document(false);
            document.Parse(chunk.memory, chunk.size);
            std::string dataAsText = document.RootElement()
                                        ->FirstChildElement("wfs:member")
                                        ->FirstChildElement("omso:PointTimeSeriesObservation")
                                        ->FirstChildElement("om:result")
                                        ->FirstChildElement("wml2:MeasurementTimeseries")
                                        ->LastChildElement("wml2:point")
                                        ->FirstChildElement("wml2:MeasurementTVP")
                                        ->FirstChildElement("wml2:value")->GetText();
            this->wind.current = "nyt tuuloo " + dataAsText + " m/s";
        }
        curl_easy_cleanup(curl_handle);
    }
    free(chunk.memory);
}

}
