#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>
#include <set>
#include <map>
#include <stdarg.h>

std::string string_format(const char* format, ...);

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive = true);
bool ends_with(const std::string& str, const std::string& end, bool case_sensitive = true);

bool split_config_line(const std::string& line, std::string& key, std::string& val);

bool in_quotes(const std::string& str);
bool remove_quotes(std::string& str);

std::set<std::string> keys(const std::map< std::string, std::set<std::string> >& map);
std::set<std::string> keys(const std::map< std::string, std::list<std::string> >& map);

std::list<std::string> split(const std::string& str, char sep);

bool between(const std::string& str, const char* from, const char* to, std::string& res);

#endif // UTILS_H
