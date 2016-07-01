#include "database.h"

Database::Database()
{

}

bool Database::addSeries(std::string data)
{
  rapidxml::xml_document<> doc;
  doc.parse<0>((char *)data.c_str());
  rapidxml::xml_node<> *node = doc.first_node()->first_node("Series");
  int seriesNumber = 0;

  while (node != 0)
  {
    seriesNumber++;
    node = node->next_sibling();
  }
  
  if (seriesNumber == 0) {
    std::cerr << "Error: Not series not found!" << std::endl;
    return false;
  }
  node = doc.first_node()->first_node("Series");

  if (seriesNumber > 1)
  {
    seriesNumber = 0;
    while (node != 0)
    {
      seriesNumber++;
      std::cerr << seriesNumber << ") ";
      std::cerr << node->first_node("SeriesName")->value() << " ";
      //std::cout << node->first_node("FirstAired")->value() << " ";
      //if (node->first_node("Network")) std::cout << "[" << node->first_node("Network")->value() << "]" << " ";
      std::cerr << std::endl;
      node = node->next_sibling();
    }

    std::cerr << std::endl << "Choose from 1-" << seriesNumber << ": ";
    int seriesChosen = -1;
    std::cin >> seriesChosen;
    if (seriesChosen <= 0 || seriesChosen > seriesNumber)
    {
      std::cerr << "Wrong number!" << std::endl;
      return false;
    }
    node = doc.first_node()->first_node("Series");
    for (int i = 1; i < seriesChosen; ++i)
    {
      node = node->next_sibling(); 
    }
  }

  std::string name = node->first_node("SeriesName")->value();
  for (size_t i = 0; i < name.size(); i++)
  {
    if ((name.at(i) < '0' || name.at(i) > 'z') && name.at(i) != ' ')
    {
      name.replace(i, 1, "");
    }
  }

  for (size_t i = 0; i < this->list.size(); ++i)
  {
    if ((*(this->list.at(i))).name == name)
    {
      std::cerr << "Error: Series already exist in database!" << std::endl;
      return false;
    }
  }

  std::string id = "";
  id = node->first_node("seriesid")->value();
  if (id.size() <= 0) id = node->first_node("id")->value();
  if (id.size() <= 0)
  {
    std::cerr << "Error: Unknown series" << std::endl;
    return false;
  }
  this->list.push_back(new Series(name, id));
  doc.clear();

  return true;
}

void Database::update()
{
  std::cerr << "Updating database..." << std::endl;

  std::string url = "";
  std::string data = "";

  for (std::vector<Series*>::iterator it = this->list.begin(); 
      it != this->list.end(); 
      ++it)
  {
    url = "";
    url += "http://thetvdb.com/api/497EF9438959C2C2/series/";
    url += (*it)->id;
    url += "/all/en.xml";
    data = Downloader::get(url);

    std::vector<Episode*> episodes2;

    rapidxml::xml_document<> doc;
    doc.parse<0>((char *)data.c_str());
    rapidxml::xml_node<> *node;
    std::cerr << "Parsing episodes of " << (*it)->name << "..." << std::endl;
   
    node = doc.first_node()->first_node("Episode");
    while (node != 0)
    {
      std::string id      = node->first_node("id")->value();
      std::string name    = node->first_node("EpisodeName")->value();
      std::string season  = node->first_node("SeasonNumber")->value();
      std::string date    = node->first_node("FirstAired")->value();
      std::string number  = node->first_node("EpisodeNumber")->value();

      if (number.size() == 1) number = "0"+number;
      if (season.size() == 1) season = "0"+season;
      //std::cout << id << " " << name << " " << season << " " << date << std::endl;
    
      //sscanf(date.c_str(), "%d-%d-%d", &(ep->year), &(ep->month), &(ep->day));
      
      int year, month, day;
      year = month = day = 9999;

      if (date.find("-") != std::string::npos)
        date = date.replace(date.find("-"), 1, " ");
      if (date.find("-") != std::string::npos)
        date = date.replace(date.find("-"), 1, " ");
      std::stringstream(date) >> year >> month >> day;
      
      if (year != 9999 && month != 9999 && day != 9999)
        episodes2.push_back(new Episode(id, number, name, year, month, day, 0, season));

      node = node->next_sibling("Episode");
    }

    /*for (size_t i = 0; i < (*it)->episodes.size(); i++)
    {
      std::cout << (*it)->episodes.at(i)->number << ")" << (*it)->episodes.at(i)->title << "| |" << (*it)->episodes.at(i)->day << "| |" << (*it)->episodes.at(i)->month << "| |" << (*it)->episodes.at(i)->year << std::endl;
    }*/

    std::sort(episodes2.begin(), 
              episodes2.end(), 
              []
        (Episode *a, Episode *b) -> bool
        {
          if (a->year != b->year)
          {
            return a->year < b->year;
          } else
          if (a->month != b->month)
          {
            return a->month < b->month;
          } else
          {
            return a->day < b->day;
          }
          return false;
        }
    );

    for (std::vector<Episode*>::iterator kt = episodes2.begin();
        kt != episodes2.end();
        kt++)    
    {
      bool exist = false;

      for (std::vector<Episode*>::iterator jt = (*it)->episodes.begin();
          jt != (*it)->episodes.end();
        jt++)
      {
        if ((*jt)->id == (*kt)->id)
        {
          exist = true;
          break;
        }
      }
      if (!exist)
      {
        std::cerr << "Adding new episode '" << (*kt)->title << "'..." << std::endl;
        (*it)->episodes.push_back((*kt));  
      }  
    }

    for (std::vector<Episode*>::iterator kt = (*it)->episodes.end()-1;
        kt != (*it)->episodes.begin()-1;
        kt--)    
    {
      bool exist = false;

      for (std::vector<Episode*>::iterator jt = episodes2.begin();
          jt != episodes2.end();
          jt++)
      {
        if ((*jt)->id == (*kt)->id)
        {
          exist = true;
          break;
        }
      }
      if (!exist)
      {
        std::cerr << "Erasing episode " << (*kt)->id << "..." << std::endl;
        (*it)->episodes.erase(kt);
      }  
    }
  }
}

void Database::save()
{
  std::cerr << "Saving changed..." << std::endl;

  rapidxml::xml_document<> doc;
  
  for (std::vector<Series*>::iterator it = this->list.begin(); 
      it != this->list.end(); 
      ++it)
  {
    rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_element, "Series");
    root->append_attribute(doc.allocate_attribute("id", (*it)->id.c_str()));
    root->append_attribute(doc.allocate_attribute("name", (*it)->name.c_str()));
    doc.append_node(root);

    for (std::vector<Episode*>::iterator jt = (*it)->episodes.begin();
        jt != (*it)->episodes.end();
        ++jt)
    {
      //std::cout << (*jt)->id << ":" << (*jt)->title << " " 
      //  << (*jt)->season << "x" << (*jt)->number << " " 
      //  << (*jt)->day << "-" << (*jt)->month << "-" << (*jt)->year << std::endl;

      rapidxml::xml_node<> *c = doc.allocate_node(rapidxml::node_element, "Episode");
      c->append_attribute(doc.allocate_attribute("id", doc.allocate_string((*jt)->id.c_str())) );
      c->append_attribute(doc.allocate_attribute("title", doc.allocate_string((*jt)->title.c_str())) );
      c->append_attribute(doc.allocate_attribute("season", doc.allocate_string((*jt)->season.c_str())) );
      c->append_attribute(doc.allocate_attribute("number", doc.allocate_string((*jt)->number.c_str())) );

      //std::cout << (*jt)->number << std::endl;
      
      c->append_attribute(doc.allocate_attribute("day", doc.allocate_string(std::to_string((*jt)->day).c_str())) );
      c->append_attribute(doc.allocate_attribute("month", doc.allocate_string(std::to_string((*jt)->month).c_str())) );
      c->append_attribute(doc.allocate_attribute("year", doc.allocate_string(std::to_string((*jt)->year).c_str())) );
      c->append_attribute(doc.allocate_attribute("seen", doc.allocate_string(std::to_string((*jt)->seen).c_str())) );

      root->append_node(c);
    }
  }

  std::string doc_str;
  rapidxml::print(std::back_inserter(doc_str), doc, 0);

  std::ofstream file;
  file.open("data.xml");

  file << doc_str;
  file.close();

  //std::cout << doc;
}

void Database::load(std::string fileName)
{
  std::ifstream file;
  file.open(fileName, std::ios::in);

  if (!file) return;

  //std::cerr << "Loading database..." << std::endl;
  
  std::string data = "";
  while (!file.eof())
  {
    char line[256];
    file.getline(line, 256);
    data += line;
  }
  file.close();

  //std::cout << data;

  rapidxml::xml_document<> doc;
  doc.parse<0>((char *)data.c_str());
  
  rapidxml::xml_node<> *node = doc.first_node();
  while (node != 0)
  {
    std::string seriesId    = node->last_attribute()->value();
    std::string seriesName  = node->first_attribute()->value();
    
    this->list.push_back(new Series(seriesId, seriesName) ); 

    rapidxml::xml_node<> *epNode = node->first_node("Episode");
    while (epNode != 0)
    {
      std::string id = epNode->first_attribute("id")->value();
      std::string title = epNode->first_attribute("title")->value();
      std::string season = epNode->first_attribute("season")->value();
      std::string number = epNode->first_attribute("number")->value();
      int day = atoi(epNode->first_attribute("day")->value());
      int month = atoi(epNode->first_attribute("month")->value());
      int year = atoi(epNode->first_attribute("year")->value());
      int seen = atoi(epNode->first_attribute("seen")->value());
      
      this->list.back()->episodes.push_back(new Episode(id, number, title, year, month, day, seen, season) );
      
      epNode = epNode->next_sibling("Episode");
    }
    
    node = node->next_sibling("Series");
  }
}

std::vector<Series*> *Database::getList()
{
  return &this->list;
}
