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

    void        clear();

    //// Global settings =======================================================

    //// Project type ----------------------------------------------------------

    Type        projectType() const;
    void        setProjectType(const Type& projectType);

    std::string projectTypeString() const;

    //// CPU family ------------------------------------------------------------

    std::string cpuFamily() const;
    void        setCpuFamily(const char* cpuFamily);

    //// Project directory -----------------------------------------------------

    std::string projectDir() const;
    void        setProjectDir(const char* projectDir);

    //// Available tools -------------------------------------------------------

    uint32_t    toolFlags() const;
    stringset   tools() const;
    cstringset& c_tools() const;

    void        addTool(const char* tool);
    void        removeTool(const char* tool);

    void        clearTools();

    //// Source list ===========================================================

    stringset   sources() const;
    cstringset& c_sources() const;

    stringset   commands() const;
    cstringset& c_commands() const;

    stringset   libraries() const;
    cstringset& c_libraries() const;

    void        addSource(const char* source);
    void        removeSource(const char* source);

    //// Configurations ========================================================

    stringset       configs() const;
    ConfigSettings  configSettings(const char* config) const;
    ConfigSettings& configSettingsRef(const char* config);

    void addConfig(const char* config);
    void removeConfig(const char* config);

    //// Build steps -----------------------------------------------------------

    void addPreBuildStep(const char* config, const char* action);
    void addPostBuildStep(const char* config, const char* action);

    void removePreBuildStep(const char* config, const char* action);
    void removePostBuildStep(const char* config, const char* action);

    void clearPreBuildSteps(const char* config);
    void clearPostBuildSteps(const char* config);

    //// Tools options ---------------------------------------------------------

    void addCompilerOption(const char* config, const char* option);
    void addLinkerOption(const char* config, const char* option);
    void addArchiverOption(const char* config, const char* option);

    //// Custom files compiler options -----------------------------------------

    void addFileOptionAdded(const char* config, const char* file, const char* option);
    void addFileOptionRemoved(const char* config, const char* file, const char* option);

    //// Files linking order ---------------------------------------------------

    void addFileLinkOrder(const char* config, const char* file, uint order);

    //// =======================================================================

private:
    Type        mType;
    std::string mCpuFamily;
    std::string mProjectDir;

    uint32_t    mToolFlags;

    configmap   mConfigs;

    stringset   mTools;
    stringset   mSources;
    stringset   mCommands;
    stringset   mLibraries;
};

#endif // PROJECTSETTINGS_H
