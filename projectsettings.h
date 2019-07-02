#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <set>
#include <map>
#include <list>

#include "configsettings.h"

typedef std::map<std::string, ConfigSettings> configmap;

class ProjectSettings
{
public:

    enum class Type
    {
        UNKNOWN,
        EXECUTABLE,
        LIBRARY,
    };

    enum ProjectType
    {
        PROJECT_UNKNOWN,
        PROJECT_EXECUTABLE,
        PROJECT_LIBRARY
    };

    enum ToolFlags
    {
        TOOL_COMPILER = 0x00000001u,
        TOOL_LINKER   = 0x00000002u,
        TOOL_ARCHIVER = 0x00000004u,
    };

public:

    ProjectSettings();

    void clear();

    Type projectType() const;
    void setProjectType(const Type &projectType);

    void addTool(const char* tool);

    void addConfig(const char* config);
    void addSource(const char* source);

    void addPreBuildStep(const char* config, const char* action);
    void addPostBuildStep(const char* config, const char* action);

    void addCompilerOption(const char* config, const char* option);
    void addLinkerOption(const char* config, const char* option);
    void addArchiverOption(const char* config, const char* option);

    void addFileOptionAdded(const char* config, const char* file, const char* option);
    void addFileOptionRemoved(const char* config, const char* file, const char* option);

    void addFileLinkOrder(const char* config, const char* file, uint order);

    stringset configs() const;
    stringset tools() const;
    stringset sources() const;
    stringset commands() const;
    stringset libraries() const;

    const stringset& c_tools() const;
    const stringset& c_sources() const;
    const stringset& c_commands() const;
    const stringset& c_libraries() const;

    uint32_t  toolFlags() const;

    ConfigSettings configSettings(const char* config) const;

private:
    Type        mType;

    uint32_t    mToolFlags;

    configmap   mConfigs;

    stringset   mTools;
    stringset   mSources;
    stringset   mCommands;
    stringset   mLibraries;
};

#endif // PROJECTSETTINGS_H
