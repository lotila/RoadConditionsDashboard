#include "apiclient.h"
#include "curlutil.h"

#include <curl/curl.h>

#include <iostream>
#include <string>

// static member initialization
CURL* APIClient::handle = nullptr;
CURLcode APIClient::responseState = CURLE_OK;
MemoryStruct APIClient::chunk =
{
    chunk.memory = nullptr,
    chunk.size = 0
};

APIClient::INIT_STATUS APIClient::initializeAPIClient()
{
    CURLcode initState = curl_global_init(CURL_GLOBAL_ALL);
    if(initState != CURLE_OK)
    {
        std::cerr << "libcurl initialization broblem: "
                  << curl_easy_strerror(initState)
                  << std::endl;
        curl_global_cleanup();
        return INIT_STATUS::NOT_OK;
    }
    handle = curl_easy_init();

    chunk.memory = (char*)malloc(1);

    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "gzip");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "RoadConditionsDashboard");
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);

    return INIT_STATUS::OK;
}

void APIClient::destructAPIClient()
{
    curl_global_cleanup();
    free(chunk.memory);
}

void APIClient::getFMIAPIData(std::string* const output, const std::string& query)
{
    if(!handle)
    {
        // TODO: throw exception
        return;
    }

    std::string url("https://opendata.fmi.fi/wfs?");
    url.append(query);

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    chunk.size = 0;
    responseState = curl_easy_perform(handle);

    if(stateIsOK())
    {
        output->append(chunk.memory, chunk.size);
    }
}

void APIClient::getDigitrafficAPIData(std::string* const output, const std::string& path, const std::string& query)
{
    if(!handle)
    {
        // TODO: throw exception
        return;
    }

    std::string url("https://tie.digitraffic.fi/api/");
    url.append(path);
    url.append("?");
    url.append(query);

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    chunk.size = 0;
    responseState = curl_easy_perform(handle);

    if (stateIsOK())
    {
        output->append(chunk.memory, chunk.size);
    }
}

bool APIClient::stateIsOK()
{
    if(responseState != CURLE_OK)
    {
        std::cerr << "fuck: " << curl_easy_strerror(responseState) << std::endl;
        return false;
    }
    return true;
}
