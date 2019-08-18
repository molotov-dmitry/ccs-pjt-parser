#include "projectsettings.h"

#include <string.h>

#include "utils.h"

ProjectSettings::ProjectSettings() : mType(Type::UNKNOWN), mToolFlags(0x00000000u)
{

}

void ProjectSettings::clear()
{
    mType = Type::UNKNOWN;

    mToolFlags = 0x00000000u;

    mConfigs.clear();
    mTools.clear();
    mSources.clear();
    mCommands.clear();
    mLibraries.clear();
}

//// Global settings ===========================================================

//// Project type --------------------------------------------------------------

ProjectSettings::Type ProjectSettings::projectType() const
{
    return mType;
}

void ProjectSettings::setProjectType(const ProjectSettings::Type& projectType)
{
    mType = projectType;
}

std::string ProjectSettings::projectTypeString() const
{
    switch (mType)
    {
    case Type::UNKNOWN:
        return "Unknown";

    case Type::EXECUTABLE:
        return "Executable";

    case Type::LIBRARY:
        return "Library";
    }

    return std::string();
}

//// CPU family ----------------------------------------------------------------

std::string ProjectSettings::cpuFamily() const
{
    return mCpuFamily;
}

void ProjectSettings::setCpuFamily(const char* cpuFamily)
{
    mCpuFamily = cpuFamily;
}

//// Project directory ---------------------------------------------------------

std::string ProjectSettings::projectDir() const
{
    return mProjectDir;
}

void ProjectSettings::setProjectDir(const char* projectDir)
{
    mProjectDir = projectDir;
}

//// Available tools -----------------------------------------------------------

uint32_t ProjectSettings::toolFlags() const
{
    return mToolFlags;
}

stringset ProjectSettings::tools() const
{
    return mTools;
}

cstringset& ProjectSettings::c_tools() const
{
    return mTools;
}

void ProjectSettings::addTool(const char* tool)
{
    if (strcasecmp(tool, "Compiler") == 0)
    {
        mToolFlags |= TOOL_COMPILER;
    }
    else if (strcasecmp(tool, "Linker") == 0)
    {
        mToolFlags |= TOOL_LINKER;
    }
    else if (strcasecmp(tool, "Archiver") == 0)
    {
        mToolFlags |= TOOL_ARCHIVER;
    }
    else
    {
        //Warning: unknown tool
    }

    mTools.insert(tool);
}

void ProjectSettings::removeTool(const char* tool)
{
    if (strcasecmp(tool, "Compiler") == 0)
    {
        mToolFlags &= (uint32_t)~TOOL_COMPILER;
    }
    else if (strcasecmp(tool, "Linker") == 0)
    {
        mToolFlags &= (uint32_t)~TOOL_LINKER;
    }
    else if (strcasecmp(tool, "Archiver") == 0)
    {
        mToolFlags &= (uint32_t)~TOOL_ARCHIVER;
    }
    else
    {
        //Warning: unknown tool
    }

    mTools.erase(tool);
}

void ProjectSettings::clearTools()
{
    mToolFlags = 0x00000000u;

    mTools.clear();
}

//// Source list ===============================================================

stringset ProjectSettings::sources() const
{
    return mSources;
}

cstringset& ProjectSettings::c_sources() const
{
    return mSources;
}

stringset ProjectSettings::commands() const
{
    return mCommands;
}

cstringset& ProjectSettings::c_commands() const
{
    return mCommands;
}

stringset ProjectSettings::libraries() const
{
    return mLibraries;
}

cstringset& ProjectSettings::c_libraries() const
{
    return mLibraries;
}

void ProjectSettings::addSource(const char* source)
{
    if (ends_with(source, ".cmd", false))
    {
        mCommands.insert(std::string(source));
    }
    else if (ends_with(source, ".lib", false))
    {
        mLibraries.insert(std::string(source));
    }
    else
    {
        mSources.insert(std::string(source));
    }
}

void ProjectSettings::removeSource(const char* source)
{
    mCommands.erase(std::string(source));
    mLibraries.erase(std::string(source));
    mSources.erase(std::string(source));
}

//// Configurations ============================================================

stringset ProjectSettings::configs() const
{
    stringset keys;

    for (auto const& element : mConfigs)
    {
        keys.insert(element.first);
    }

    return keys;
}

ConfigSettings ProjectSettings::configSettings(const char* config) const
{
    if (mConfigs.find(config) != mConfigs.end())
    {
        return mConfigs.at(config);
    }
    else
    {
        return ConfigSettings();
    }
}

ConfigSettings& ProjectSettings::configSettingsRef(const char* config)
{
    return mConfigs[config];
}

void ProjectSettings::addConfig(const char* config)
{
    mConfigs.insert(std::make_pair(std::string(config), ConfigSettings()));
}

void ProjectSettings::removeConfig(const char* config)
{
    mConfigs.erase(config);
}

//// Build steps ---------------------------------------------------------------

void ProjectSettings::addPreBuildStep(const char* config, const char* action)
{
    mConfigs[config].addPreBuildStep(action);
}

void ProjectSettings::addPostBuildStep(const char* config, const char* action)
{
    mConfigs[config].addPostBuildStep(action);
}

void ProjectSettings::removePreBuildStep(const char* config, const char* action)
{
    mConfigs[config].removePreBuildStep(action);
}

void ProjectSettings::removePostBuildStep(const char* config, const char* action)
{
    mConfigs[config].removePostBuildStep(action);
}

void ProjectSettings::clearPreBuildSteps(const char* config)
{
    mConfigs[config].clearPreBuildSteps();
}

void ProjectSettings::clearPostBuildSteps(const char* config)
{
    mConfigs[config].clearPostBuildSteps();
}

//// Tools options -------------------------------------------------------------

void ProjectSettings::addCompilerOption(const char* config, const char* option)
{
    mConfigs[config].addCompilerOption(option);
}

void ProjectSettings::addLinkerOption(const char* config, const char* option)
{
    mConfigs[config].addLinkerOption(option);
}

void ProjectSettings::addArchiverOption(const char* config, const char* option)
{
    mConfigs[config].addArchiverOption(option);
}

//// Custom files compiler options ---------------------------------------------

void ProjectSettings::addFileOptionAdded(const char* config, const char* file, const char* option)
{
    mConfigs[config].addFileOptionAdded(file, option);
}

void ProjectSettings::addFileOptionRemoved(const char* config, const char* file, const char* option)
{
    mConfigs[config].addFileOptionRemoved(file, option);
}

//// Files linking order -------------------------------------------------------

void ProjectSettings::addFileLinkOrder(const char* config, const char* file, uint order)
{
    mConfigs[config].addFileLinkOrder(file, order);
}
