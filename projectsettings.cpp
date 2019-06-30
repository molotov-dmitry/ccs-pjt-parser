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

ProjectSettings::Type ProjectSettings::projectType() const
{
    return mType;
}

void ProjectSettings::setProjectType(const Type& projectType)
{
    mType = projectType;
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

void ProjectSettings::addConfig(const char* config)
{
    mConfigs.insert(std::make_pair(std::string(config), ConfigSettings()));
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

void ProjectSettings::addPreBuildStep(const char* config, const char* action)
{
    mConfigs[config].addPreBuildStep(action);
}

void ProjectSettings::addPostBuildStep(const char* config, const char* action)
{
    mConfigs[config].addPostBuildStep(action);
}

void ProjectSettings::addCompilerOption(const char* config, const char* option)
{
    mConfigs[config].addCompilerOption(option);
}

void ProjectSettings::addLinkerOption(const char* config, const char* option)
{
    mConfigs[config].addLinkerOption(option);
}

void ProjectSettings::addArchiverOption(const char *config, const char *option)
{
    mConfigs[config].addArchiverOption(option);
}

void ProjectSettings::addFileOptionAdded(const char* config, const char* file, const char* option)
{
    mConfigs[config].addFileOptionAdded(file, option);
}

void ProjectSettings::addFileOptionRemoved(const char* config, const char* file, const char* option)
{
    mConfigs[config].addFileOptionRemoved(file, option);
}

void ProjectSettings::addFileLinkOrder(const char* config, const char *file, uint order)
{
    mConfigs[config].addFileLinkOrder(file, order);
}

stringset ProjectSettings::tools() const
{
    return mTools;
}

stringset ProjectSettings::sources() const
{
    return mSources;
}

stringset ProjectSettings::commands() const
{
    return mCommands;
}

stringset ProjectSettings::libraries() const
{
    return mLibraries;
}

const stringset& ProjectSettings::c_tools() const
{
    return mTools;
}

const stringset& ProjectSettings::c_sources() const
{
    return mSources;
}

const stringset& ProjectSettings::c_commands() const
{
    return mCommands;
}

const stringset& ProjectSettings::c_libraries() const
{
    return mLibraries;
}
