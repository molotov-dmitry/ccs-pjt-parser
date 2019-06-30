#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>
#include <stdarg.h>

std::string string_format(const char *format, ...);

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive = true);
bool ends_with(const std::string& str, const std::string& end, bool case_sensitive = true);

bool split_config_line(const std::string& line, std::string& key, std::string& val);

bool in_quotes(const std::string& str);
bool remove_quotes(std::string& str);

std::list<std::string> split(const std::string& str, char sep);

bool between(const std::string& str, const char* from, const char* to, std::string& res);

#endif // UTILS_H
