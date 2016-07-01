#include "series.h"

Episode::Episode(std::string id, std::string number, std::string title, int year, int month, int day, int seen, std::string season)
{
  this->id = id;
  this->title = title;
  this->year = year;
  this->month = month;
  this->day = day;
  this->season = season;
  this->seen = seen;
  this->number = number;
}

Series::Series(std::string _name, std::string _id)
{
  this->name = _name;
  this->id = _id;
}
