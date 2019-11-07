#include "utils.h"

#include <memory>
#include <string.h>

std::string string_format(const char* format, ...)
{
    va_list args, args_size;
    va_start(args, format);
    va_copy(args_size, args);

    int size = vsnprintf(nullptr, 0, format, args_size) + 1;
    if (size < 0)
    {
        va_end(args_size);
        va_end(args);

        return std::string();
    }

    va_end(args_size);

    std::unique_ptr<char[]> buf(new char[size]);

    vsnprintf(buf.get(), (size_t)size, format, args);

    va_end(args);

    return std::string(buf.get(), buf.get() + size - 1);
}

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive)
{
    if (case_sensitive)
    {
        return (strncasecmp(str.c_str(), start.c_str(), start.length()) == 0);
    }
    else
    {
        return (strncmp(str.c_str(), start.c_str(), start.length()) == 0);
    }
}

bool ends_with(const std::string& str, const std::string& end, bool case_sensitive)
{
    if (end.length() > str.length())
    {
        return false;
    }

    std::size_t offset = str.length() - end.length();

    if (case_sensitive)
    {
        return (strcasecmp(str.substr(offset).c_str(), end.c_str()) == 0);
    }
    else
    {
        return (strcmp(str.substr(offset).c_str(), end.c_str()) == 0);
    }
}

bool split_config_line(const std::string& line, std::string& key, std::string& val)
{
    std::size_t pos = line.find('=');

    if (pos == std::string::npos)
    {
        return false;
    };

    key = line.substr(0, pos);
    val = line.substr(pos + 1);

    if (in_quotes(val))
    {
        remove_quotes(val);
    }

    return true;
}

bool in_quotes(const std::string& str)
{
    if (str.length() >= 2 && starts_with(str, "\"") && ends_with(str, "\""))
    {
        return true;
    }

    return false;
}

bool remove_quotes(std::string& str)
{
    if (in_quotes(str))
    {
        str = str.substr(1, str.length() - 2);
        return true;
    }

    return false;
}

std::set<std::string> keys(const std::map<std::string, std::set<std::string> > &map)
{
    std::set<std::string> result;

    for (auto it = map.begin(); it != map.end(); ++it)
    {
        result.insert(it->first);
    }

    return result;
}

std::set<std::string> keys(const std::map<std::string, std::list<std::string> > &map)
{
    std::set<std::string> result;

    for (auto it = map.begin(); it != map.end(); ++it)
    {
        result.insert(it->first);
    }

    return result;
}

std::set<std::string> keys(const std::map<std::string, FileOptions>& map)
{
    std::set<std::string> result;

    for (auto it = map.begin(); it != map.end(); ++it)
    {
        result.insert(it->first);
    }

    return result;
}

std::string join(const std::list<std::string> &list, char sep)
{
    size_t listsize = 0;
    std::string result;

    for (const std::string& str : list)
    {
        listsize += str.size() + 1;
    }

    result.reserve(listsize);

    for (const std::string& str : list)
    {
        result.append(str);
        result.append(std::string(&sep, 1));
    }

    if (not result.empty())
    {
        result.pop_back();
    }

    return result;
}

std::string join(const std::set<std::string> &list, char sep)
{
    size_t listsize = 0;
    std::string result;

    for (const std::string& str : list)
    {
        listsize += str.size() + 1;
    }

    result.reserve(listsize);

    for (const std::string& str : list)
    {
        result.append(str);
        result.append(std::string(&sep, 1));
    }

    if (not result.empty())
    {
        result.pop_back();
    }

    return result;
}

std::list<std::string> split(const std::string& str, char sep)
{
    std::list<std::string> result;

    std::size_t pos = 0;

    while (pos != std::string::npos)
    {
        std::size_t end = str.substr(pos).find(sep);

        result.push_back(str.substr(pos, end));

        if (end == std::string::npos)
        {
            break;
        }
        else
        {
            pos += end + 1;
        }
    }

    return result;
}

bool between(const std::string& str, const char* from, const char* to, std::string& res)
{
    std::size_t start_pos = str.find(from);

    if (start_pos != std::string::npos)
    {
        std::size_t len = str.substr(start_pos + strlen(from)).find(to);

        if (len == std::string::npos)
        {
            return false;
        }

        res = str.substr(start_pos + strlen(from), len);

        return true;
    }

    return false;
}
