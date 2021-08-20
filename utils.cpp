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

bool is_flag(const std::string& option, const char* flag, std::string& value)
{
    size_t flaglen = strlen(flag);

    if (option.length() >= flaglen + 2)
    {
        if (starts_with(option, flag) &&
            option[flaglen] == '"' &&
            option.back() == '"')
        {
            value = option.substr(flaglen + 1, option.length() - 2 - flaglen);
            return true;
        }
    }

    return false;
}

bool split_config_line(const std::string& line, std::string& key, std::string& val)
{
    std::size_t pos = line.find('=');

    if (pos == std::string::npos)
    {
        return false;
    }

    key = line.substr(0, pos);
    val = line.substr(pos + 1);

    if (in_quotes(val))
    {
        remove_quotes(val);
    }

    return true;
}

std::string replace(std::string str, char from, char to)
{
    size_t len = str.size();

    for (size_t i = 0; i < len; ++i)
    {
        if (str[i] == from)
        {
            str[i] = to;
        }
    }

    return str;
}

std::string fixpath(std::string str)
{
    return replace(str, '\\', '/');
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

    if (str.empty())
    {
        return result;
    }

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

std::string to_option(const std::string& flag, const std::string& value, bool quote)
{
    std::string option;
    option.reserve(flag.length() + value.length() + 2);

    option.append(flag);

    if (quote)
    {
        option.append("\"");
    }

    option.append(value);

    if (quote)
    {
        option.append("\"");
    }

    return option;
}

std::string to_upper(std::string s)
{
    size_t len = s.size();

    for (size_t i = 0; i < len; ++i)
    {
        s[i] = (char)toupper(s[i]);
    }

    return s;
}

std::string to_lower(std::string s)
{
    size_t len = s.size();

    for (size_t i = 0; i < len; ++i)
    {
        s[i] = (char)tolower(s[i]);
    }

    return s;
}

std::list<std::string> to_lower(const std::set<std::string>& s)
{
    std::list<std::string> result;

    for (const std::string& str : s)
    {
        result.push_back(to_lower(str));
    }

    return result;
}

std::string cp1251_to_unicode(std::string cp1251)
{
    static const short utf[256] =
    {
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
        0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
        0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
        0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
        0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
        0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
        0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
        0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
        0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
        0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
        0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
        0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
        0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
        0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
        0x0402, 0x0403, 0x201a, 0x0453, 0x201e, 0x2026, 0x2020, 0x2021,
        0x20ac, 0x2030, 0x0409, 0x2039, 0x040a, 0x040c, 0x040b, 0x040f,
        0x0452, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
        0x0000, 0x2122, 0x0459, 0x203a, 0x045a, 0x045c, 0x045b, 0x045f,
        0x00a0, 0x040e, 0x045e, 0x0408, 0x00a4, 0x0490, 0x00a6, 0x00a7,
        0x0401, 0x00a9, 0x0404, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x0407,
        0x00b0, 0x00b1, 0x0406, 0x0456, 0x0491, 0x00b5, 0x00b6, 0x00b7,
        0x0451, 0x2116, 0x0454, 0x00bb, 0x0458, 0x0405, 0x0455, 0x0457,
        0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
        0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
        0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
        0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
        0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
        0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
        0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
        0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f
    };

    std::string result;

    for (char c1251 : cp1251)
    {
        short c = utf[(unsigned char)c1251];
        if (c < 0x80)
        {
            result.push_back((char)c);
        }
        else if (c < 0x800)
        {
            result.push_back((char)(c >> 6 | 0xc0));
            result.push_back((char)((c & 0x3f) | 0x80));
        }
    }

    return result;
}


std::string basename(const std::string& path)
{
    std::list<std::string> pathlist = split(path, '/');

    if (pathlist.size() > 1)
    {
        return pathlist.back();
    }

    return path;
}
