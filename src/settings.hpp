#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>

struct settings{
	std::string file;
	int level;
	std::set<std::string> modules;
	void load(const std::string &filename);
	void save(const std::string &filename);
};

