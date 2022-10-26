#ifndef APICLIENT_H
#define APICLIENT_H

#include <curl/curl.h>

#include <string>

class APIClient
{
public:
    enum class INIT_STATUS
    {
        OK,
        NOT_OK
    };
    static INIT_STATUS initializeAPIClient();
    static void destructAPIClient();

    static void getFMIAPIData(std::string* const output, const std::string& query);
    static void getDigitrafficAPIData(std::string* const output, const std::string& path, const std::string& query);
private:
    APIClient() = default;
    static bool stateIsOK();
    static CURL* handle;
    static CURLcode responseState;
    static struct MemoryStruct chunk;
};

#endif
