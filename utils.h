#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>
#include <set>
#include <map>
#include <stdarg.h>

#include "fileoptions.h"

std::string string_format(const char* format, ...);

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive = true);
bool ends_with(const std::string& str, const std::string& end, bool case_sensitive = true);

bool split_config_line(const std::string& line, std::string& key, std::string& val);

bool in_quotes(const std::string& str);
bool remove_quotes(std::string& str);

std::set<std::string> keys(const std::map< std::string, std::set<std::string> >& map);
std::set<std::string> keys(const std::map< std::string, std::list<std::string> >& map);
std::set<std::string> keys(const std::map< std::string,  FileOptions>& map);

std::string join(const std::list<std::string>& list, char sep);
std::string join(const std::set<std::string>& list, char sep);

std::list<std::string> split(const std::string& str, char sep);

bool between(const std::string& str, const char* from, const char* to, std::string& res);

std::string to_option(const std::string& flag, const std::string& value, bool quote);

std::string to_upper(std::string s);
std::string to_lower(std::string s);

std::list<std::string> to_lower(const std::set<std::string>& s);

std::string cp1251_to_unicode(std::string cp1251);

#endif // UTILS_H
