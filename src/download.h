#pragma once

#include <curl/curl.h>
#include <string>

class Downloader
{
  public:
    static void save(std::string url, std::string fileName);
    static std::string get(std::string url);
    static bool init();
    static size_t writeFunction(char *ptr, size_t size, size_t nmemb, void *user_data);    
    static std::string encodeUrl(std::string url);

    static CURL *curl;    
};
