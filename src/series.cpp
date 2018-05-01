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

Episode::Episode(const Episode &ep)
{
  this->id = ep.id;
  this->title = ep.title;
  this->year = ep.year;
  this->month = ep.month;
  this->day = ep.day;
  this->season = ep.season;
  this->seen = ep.seen;
  this->number = ep.number;
}

Series::Series(std::string _name, std::string _id)
{
  this->name = _name;
  this->id = _id;
}

Series::~Series()
{
  for (size_t i = 0; i < this->episodes.size(); i++)
  {
    delete (this->episodes.at(i));
  }
}
