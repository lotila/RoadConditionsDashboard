#ifndef CURLUTIL_H
#define CURLUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, char* userp);

#endif
