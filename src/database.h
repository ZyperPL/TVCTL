#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <algorithm>

#include "series.h"
#include "download.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

class Database
{
  private:
    std::vector<Series*> list;
  public:
    Database();
    bool addSeries(std::string data);
    void update();
    void save();
    void load(std::string fileName);

    std::vector<Series*> *getList();
};
