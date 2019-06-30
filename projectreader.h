#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include <string>
#include <projectsettings.h>
#include <projectparser.h>

class ProjectReader
{
public:
    ProjectReader(const char* path = nullptr);

    bool read(const char* path = nullptr);

    std::string lastError() const;

    ProjectSettings projectSettings() const;

private:

//    ProjectParser mParser;
    ProjectSettings mSettings;
    std::string     mPath;
    std::string     mLastError;

    static void removeLineFeeds(char* string, size_t& length);
};

#endif // PROJECTREADER_H
