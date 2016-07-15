#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>

#include "download.h"
#include "series.h"
#include "database.h"

void help(char *name)
{
  std::cerr << "Usage: " << std::endl;
  std::cerr << name << " " << "[command] [parameter]" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Commands: " << std::endl;
  std::cerr << ">  add \"[series_name]\"\t - adds new TV series to database" << std::endl;
  std::cerr << "\tseries_name\t - TV series name" << std::endl;

  std::cerr << ">  remove \"[series_name]\"\t - removes TV series from database" << std::endl;
  std::cerr << "\tseries_name\t - TV series name added previously" << std::endl;

  std::cerr << ">  see \"[series_name]\" [SxxEyy]\t - marks an episode as seen" << std::endl;
  std::cerr << "\tseries_name\t - TV series name" << std::endl;
  std::cerr << "\tSxxEyy\t\t - season XX number and episode YY number" << std::endl;

  std::cerr << ">  new \"[series_name]\" [SxxEyy]\t - marks an episode as seen" << std::endl;
  std::cerr << "\tseries_name\t - TV series name" << std::endl;
  std::cerr << "\tSxxEyy\t\t - season XX number and episode YY number" << std::endl;
  
  std::cerr << ">  check (info) (seen)\t - prints unseen(/seen) episodes" << std::endl;
  std::cerr << "\tinfo\t - optional: prints extra info" << std::endl;
  std::cerr << "\tseen\t - optional: prints seen episodes" << std::endl;

  std::cerr << ">  update\t - force database update" << std::endl;
  std::cerr << std::endl;
  exit(-1);
}

int main (int argc, char *argv[])
{
  if (argc <= 1)
  {
    help(argv[0]);
  }

  std::string command = argv[1];
  std::transform(command.begin(), command.end(), command.begin(), ::tolower);
  std::string parameter = "";
  for(int i = 2; i < argc; ++i)
  {
    parameter += argv[i];
    parameter += " ";
  }
  if (parameter.size() > 1) {
    parameter.erase(parameter.size()-1);
  }

  if (argc > 1)
  {
    if (command == "--help" || command == "-h" || command == "help")
    {
      help(argv[0]);
    }
  }

  time_t t = time(0);
  struct tm *currentTime = localtime(&t);

  Database *database = new Database();
  database->load("data.xml");

  if (command == "add")
  {
    std::cerr << "Searching for " << parameter << "..." << std::endl; 
    std::string url = "http://thetvdb.com/api/GetSeries.php?seriesname=";
    parameter = Downloader::encodeUrl(parameter);
    url += parameter;
    bool ret = database->addSeries(Downloader::get(url));      
    if (!ret) return 1;
    
    database->update();     
    database->save();
  } else
  if (command == "remove")
  {
    std::cerr << "Removing " << parameter << "..." << std::endl; 
    bool ret = false;
    for(std::vector<Series*>::iterator it = database->getList()->begin();
        it != database->getList()->end();
        it++)
    {
      if ((*it)->name == parameter)
      {
        std::cerr << "Series '" << parameter << "' removed.\n";
        database->getList()->erase(it);
        ret = true;
        break;
      }
    }
    if (!ret) return 1;
    
    database->update();     
    database->save();
  } else
  if (command == "see" || command == "new")
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

    for(std::vector<Series*>::iterator it = database->getList()->begin();
        it != database->getList()->end();
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

    database->save();
  } else
  if (command == "check"
  ||  command == "c")
  {
    std::string parameter2 = parameter.substr(parameter.find_last_of(" ")+1);
    parameter = parameter.substr(0, parameter.find_last_of(" "));

    for(std::vector<Series*>::iterator it = database->getList()->begin();
        it != database->getList()->end();
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
  } else
  if (command == "update"
  ||  command == "u")
  {
    database->update();
    database->save();
  } else
  {
    std::cerr << "Unknown " << argv[0] << " command: " << command << std::endl;
  }

  return 0;
}
