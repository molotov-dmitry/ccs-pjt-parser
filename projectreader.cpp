#include "projectreader.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"

ProjectReader::ProjectReader(const char* path)
{
    if (path != nullptr)
    {
        mPath = path;
    }
}

bool ProjectReader::read(const char* path)
{
    if (path != nullptr)
    {
        mPath = path;
    }

    mLastError.clear();

    //// Check argument count ==================================================

    if (mPath.empty())
    {
        mLastError = "Missing project path argument";
        return false;
    }

    //// Read project file =====================================================

    size_t projectSize   = 0;
    char*  projectBuffer = nullptr;

    {
        FILE* projectFile = fopen(mPath.c_str(), "r");
        if (projectFile == nullptr)
        {
            mLastError = string_format("Failed to open project '%s': '%s'", mPath.c_str(), strerror(errno));
            return false;
        }

        struct stat projectStat;

        if (fstat(fileno(projectFile), &projectStat) != 0)
        {
            mLastError = string_format("Failed to get project size: '%s'", strerror(errno));
            fclose(projectFile);
            return false;
        }

        projectBuffer = new char[projectStat.st_size + 2];
        if (projectBuffer == nullptr)
        {
            mLastError = "Failed to allocate memory";
            fclose(projectFile);
            return false;
        }

        memset(projectBuffer, 0x00, (size_t)(projectStat.st_size + 2));

        projectSize = fread(projectBuffer, 1, (size_t)projectStat.st_size, projectFile);

        fclose(projectFile);

        removeLineFeeds(projectBuffer, projectSize);

        char* cr = strchr(projectBuffer, '\r');

        if (cr != nullptr)
        {
            mLastError = "Failed to remove CR";
            return false;
        }
    }

    //// Parse project =========================================================

    ProjectParser parser;

    char* nextLine = nullptr;

    for (char* currentLine = projectBuffer; currentLine[0] != '\0'; currentLine = nextLine + 1)
    {
        //// -------------------------------------------------------------------

        while (currentLine[0] == ' ' || currentLine[0] == '\t')
        {
            ++currentLine;
        }

        nextLine = strchrnul(currentLine, '\n');
        size_t lineLength = (size_t)(nextLine - currentLine);

        int shift = -1;

        while (nextLine[shift] == ' ' || nextLine[shift] == '\t' && lineLength > 0)
        {
            --lineLength;
            --shift;
        }

        currentLine[lineLength] = '\0';

        //// Check for comment or new line -------------------------------------

        if (lineLength == 0 || currentLine[0] == ';')
        {
            continue;
        }

        //// Check for new section ---------------------------------------------

        if (not parser.parseLine(currentLine))
        {
            mLastError = parser.lastError().c_str();
            return false;
        }

        //// -------------------------------------------------------------------

    }

    mSettings = parser.projectSettings();

    //// =======================================================================

    return true;
}

std::string ProjectReader::lastError() const
{
    return mLastError;
}

ProjectSettings ProjectReader::projectSettings() const
{
    return mSettings;
}

void ProjectReader::removeLineFeeds(char* string, size_t& length)
{
    size_t lineFeeds = 0;

    char* lastLineFeed = strchrnul(string, '\r');

    while (true)
    {
        char* nextLineFeed = strchrnul(lastLineFeed + 1, '\r');

        if (nextLineFeed[0] == '\0')
        {
            break;
        }

        memmove(lastLineFeed - lineFeeds, lastLineFeed + 1, (size_t)(nextLineFeed - lastLineFeed));

        ++lineFeeds;

        lastLineFeed = nextLineFeed;
    }

    memset(string + length - lineFeeds - 2, 0x00, lineFeeds);

    length -= lineFeeds;
}
