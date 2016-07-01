#pragma once

#include <string>
#include <vector>

struct Episode
{
  Episode(std::string id, std::string number, std::string title, int year, int month, int day, int seen, std::string season);
  std::string id;
  std::string number;
  std::string title;
  int year;
  int month;
  int day;
  int seen;
  std::string season;
};

class Series
{
  public:
    Series(std::string _name, std::string _id);
    std::string name;
    std::string id;
    std::vector<Episode*> episodes; 
};
