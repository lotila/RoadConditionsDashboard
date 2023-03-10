#include "curlutil.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, char* userp) {
    size_t realsize = size * nmemb;
    MemoryStruct* mem = (MemoryStruct *)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("error: not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
