#include <curl/curl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "download.h"

CURL *Downloader::curl = NULL;

bool Downloader::init()
{
  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    return 0;
  }
  return 1;
}

void Downloader::save(std::string url, std::string fileName)
{
  if (!Downloader::curl)
  {
    Downloader::init();
  }

  std::ofstream file;
  file.open(fileName.c_str(), std::ios::out);

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  curl_easy_setopt(curl, CURLOPT_FILE, &file);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
  {
    std::cerr << "ERROR: " << curl_easy_strerror(res) << std::endl;
  } 
  curl_easy_reset(curl);
}

std::string Downloader::get(std::string url)
{
  if (!Downloader::curl)
  {
    Downloader::init();
  }

  //std::cout << "Downloading " << url << "... " << std::endl;
  std::stringstream stream;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_FILE, &stream);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
  curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_NONE);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
  {
    std::cerr << "ERROR: " << curl_easy_strerror(res) << std::endl;
  } 
  curl_easy_reset(curl);

  return stream.str();
}

std::string Downloader::encodeUrl(std::string url)
{
  char *url_encoded = curl_easy_escape(curl, url.c_str(), (int)url.size());
  std::string returnUrl = url_encoded;
  curl_free(url_encoded);
  return returnUrl;
}

size_t Downloader::writeFunction(char *ptr, size_t size, size_t nmemb, void *user_data)
{
  std::ostream &file = *(std::ostream*)(user_data);

  file.write(ptr, size*nmemb);
  
  return size*nmemb;
}
