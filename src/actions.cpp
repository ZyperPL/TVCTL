#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <functional>

#include "series.h"
#include "database.h"
#include "download.h"
#include "actions.h"

Action::Action(std::string command)
{
  const char *commands[] = { 
    "add",
    "remove", 
    "see", 
    "new", 
    "check", 
    "update" 
  };
  const std::function<bool(Database*, std::string)> functions[] = 
  {
    Action::addToDatabase, 
    Action::removeFromDatabase, 
    Action::markAsSeen, 
    Action::markAsNew, 
    Action::checkInDatabase, 
    Action::updateDatabase
  };

  const int COMMAND_LENGTH = 6;

  execute = NULL;
  for (size_t i = 0; i < COMMAND_LENGTH; i++)
  {
    if (commands[i] == command) {
      execute = functions[i];
      break;
    }
  }
}

bool Action::addToDatabase(Database *db, std::string parameter)
{
  std::cerr << "Searching for " << parameter << "..." << std::endl; 
  std::string url = "http://thetvdb.com/api/GetSeries.php?seriesname=";
  parameter = Downloader::encodeUrl(parameter);
  url += parameter;
  bool ret = db->addSeries(Downloader::get(url)); 

  if (!ret) return false;

  db->update();     
  db->save();

  return true;
}


bool Action::removeFromDatabase(Database *db, std::string parameter)
{
  std::cerr << "Removing " << parameter << "..." << std::endl; 
  bool ret = false;
  for(std::vector<Series*>::iterator it = db->getList()->begin();
      it != db->getList()->end();
      it++)
  {
    if ((*it)->name == parameter)
    {
      std::cerr << "Series '" << parameter << "' removed.\n";
      db->getList()->erase(it);
      ret = true;
      break;
    }
  }
  if (!ret) return false;

  db->update();     
  db->save();

  return true;
}

bool Action::markInDatabase(Database *db, std::string parameter, std::string command)
{
  std::string parameter2 = parameter.substr(parameter.find_last_of(" ")+1);
  std::string season = parameter2;
  std::string number = parameter2;
  if (parameter2 != "*")
  {
    season = season.substr(1, season.find("E")-1);
    number = number.substr(number.find("E")+1);
  } else
  {
    season = "*";
    number = "*";
  }
  parameter = parameter.substr(0, parameter.find_last_of(" "));

  for(std::vector<Series*>::iterator it = db->getList()->begin();
      it != db->getList()->end();
      it++)
  {
    if ((*it)->name == parameter || parameter == "*")
    {
      for (std::vector<Episode*>::iterator jt = (*it)->episodes.begin();
            jt != (*it)->episodes.end();
            jt++)
      {
        if ((*jt)->season == season || season == "*")
        if ((*jt)->number == number || number == "*")
        {
          std::cout << "Marking " << (*jt)->title;
          if (command == "see")
          {
            std::cout << " as seen." << std::endl;
            (*jt)->seen = 1;
          } else
          {
            std::cout << " as unseen." << std::endl;
            (*jt)->seen = 0;
          }
        }
      }
    }
  }

  db->save();

  return true;
}

bool Action::markAsSeen(Database *db, std::string parameter)
{
  return markInDatabase(db, parameter, "see");
}

bool Action::markAsNew(Database *db, std::string parameter)
{
  return markInDatabase(db, parameter, "new");
}


bool Action::checkInDatabase(Database *db, std::string parameter)
{
  std::string parameter2 = parameter.substr(parameter.find_last_of(" ")+1);
  parameter = parameter.substr(0, parameter.find_last_of(" "));

  time_t t = time(NULL);
  struct tm *currentTime = localtime(&t);

  for(std::vector<Series*>::iterator it = db->getList()->begin();
      it != db->getList()->end();
      it++)
  {
    for (std::vector<Episode*>::iterator jt = (*it)->episodes.begin();
        jt != (*it)->episodes.end();
        jt++)
    {
      int ty = currentTime->tm_year+1900;
      int tm = currentTime->tm_mon+1;
      int td = currentTime->tm_mday;
      if ((*jt)->year == ty)
      {
        if ((*jt)->month == tm)
        {
          if ((*jt)->day > td)
          {
            continue;
          }
        } else
        if ((*jt)->month > tm)
        {
          continue;
        }
      } else
      if ((*jt)->year > ty)
      {
        continue;
      }

      bool yes = false;
      if (parameter == "seen" || parameter2 == "seen") yes = true;
      if ((*jt)->seen == yes)
      {
        std::cout << (*it)->name << " ";
        std::cout << "S" << (*jt)->season << "E" << (*jt)->number;
        std::cout << ": " << (*jt)->title;
        if (parameter == "info" || parameter2 == "info")
        {
          std::cout << " [" << (*jt)->day << "-";
          std::cout << (*jt)->month << "-";
          std::cout << (*jt)->year << "]";
        }

        std::cout << std::endl;
      }
    
    }
  }

  return true;
}

bool Action::updateDatabase(Database *db, std::string parameter)
{
  (void)(parameter); // prevents warning

  bool ret = db->update();
  if (ret) db->save();

  return true;
}
