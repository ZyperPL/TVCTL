#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>


#include "download.h"
#include "series.h"
#include "database.h"
#include "actions.h"

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

  Database *database = new Database();
  database->load("data.xml");

  Action action(command);

  bool executed = false;
  
  if (action.execute)
  {
    executed = action.execute(database, parameter);

    if (!executed)
    {
      help(argv[0]);
      std::cerr << "Wrong parameters!\n" << std::endl;
      return 1;
    }
  } else
  {
    std::cerr << "Unknown " << argv[0] << " command: " << command << std::endl;
    return 2;
  }

  return 0;
}
