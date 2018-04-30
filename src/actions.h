#pragma once

#include <string>
#include <functional>

class Database;

class Action
{
  public:
    Action(std::string command);
    std::function<bool(Database*, std::string)> execute;

  private:
    static bool addToDatabase(Database *db, std::string parameter);
    static bool removeFromDatabase(Database *db, std::string parameter);
    static bool markInDatabase(Database *db, std::string parameter, std::string command);
    static bool markAsSeen(Database *db, std::string parameter);
    static bool markAsNew(Database *db, std::string parameter);
    static bool checkInDatabase(Database *db, std::string parameter);
    static bool updateDatabase(Database *db, std::string parameter);
};

