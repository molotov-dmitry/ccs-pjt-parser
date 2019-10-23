#ifndef PROJECTPARSER_H
#define PROJECTPARSER_H

#include "projectsettings.h"

class ProjectParser
{
public:

    enum class SectionType
    {
        NONE,
        PROJECT_SETTINGS,
        SOURCE_FILES,
        CONFIG_SETTINGS,
        TOOL_SETTINGS,
        SOURCE_SETTINGS,
        LIBRARY_SETTINGS,
        COMMAND_SETTINGS
    };

public:
    ProjectParser();

    bool parseLine(const char* line_c);

    void clear();

    std::string lastError() const;

    ProjectSettings projectSettings() const;

private:

    SectionType     mSectionType;
    ProjectSettings mProjectSettings;

    std::string     mCurrentConfig;
    std::string     mCurrentTool;
    std::string     mCurrentFile;

    std::string     mLastError;

    bool isSection(const std::string& line) const;
    bool isConfigSettingsSection(const std::string& line, std::string& config) const;

    bool getFileSettingsSection(const std::string& line, std::string& config, std::string& file, const stringset& file_set) const;

    bool isToolSettingsSection(const std::string& line, std::string& config, std::string& tool) const;
    bool isSourceSettingsSection(const std::string& line, std::string& config, std::string& source) const;
    bool isLibrarySettingsSection(const std::string& line, std::string& config, std::string& library) const;
    bool isCommandSettingsSection(const std::string& line, std::string& config, std::string& command) const;

    bool parseSection(const std::string& line);
    bool parseData(const std::string& line);

    bool parseProjectSettings(const std::string& key, const std::string& value);
    bool parseSourceFile(const std::string& key, const std::string& value);
    bool parseConfigSettings(const std::string& key, const std::string& value);
    bool parseToolSettings(const std::string& key, const std::string& value);
    bool parseSourceSettings(const std::string& key, const std::string& value);
};

#endif // PROJECTPARSER_H
