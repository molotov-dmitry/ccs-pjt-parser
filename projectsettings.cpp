﻿#include "projectsettings.h"

#include <string.h>

#include "utils.h"

ProjectSettings::ProjectSettings() : mType(Type::UNKNOWN), mToolFlags(0x00000000u)
{

}

ProjectSettings::ProjectSettings(const ProjectSettings& other) :
    mType(other.mType),
    mCpuFamily(other.mCpuFamily),
    mProjectDir(other.mProjectDir),
    mToolFlags(other.mToolFlags),
    mConfigs(other.mConfigs),
    mTools(other.mTools),
    mSources(other.mSources),
    mCommands(other.mCommands),
    mLibraries(other.mLibraries)
{

}

ProjectSettings&ProjectSettings::operator=(const ProjectSettings& other)
{
    this->mType       = other.mType;

    this->mCpuFamily  = other.mCpuFamily;
    this->mProjectDir = other.mProjectDir;

    this->mToolFlags  = other.mToolFlags;

    this->mConfigs    = other.mConfigs;

    this->mTools      = other.mTools;
    this->mSources    = other.mSources;
    this->mCommands   = other.mCommands;
    this->mLibraries  = other.mLibraries;

    return *this;
}

bool ProjectSettings::operator==(const ProjectSettings& other) const
{
    if (this->mType != other.mType)
    {
        return false;
    }

    if (this->mCpuFamily != other.mCpuFamily)
    {
        return false;
    }

    if (this->mProjectDir != other.mProjectDir)
    {
        return false;
    }

    if (this->mToolFlags != other.mToolFlags)
    {
        return false;
    }

    if (this->mConfigs != other.mConfigs)
    {
        return false;
    }

    if (this->mTools != other.mTools)
    {
        return false;
    }

    if (this->mSources != other.mSources)
    {
        return false;
    }

    if (this->mCommands != other.mCommands)
    {
        return false;
    }

    if (this->mLibraries != other.mLibraries)
    {
        return false;
    }

    return true;
}

bool ProjectSettings::operator!=(const ProjectSettings& other) const
{
    return !(*this == other);
}

void ProjectSettings::clear()
{
    mType = Type::UNKNOWN;

    mCpuFamily.clear();
    mProjectDir.clear();

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

stringset ProjectSettings::files() const
{
    std::set<std::string> allSources;

    allSources.insert(mSources.begin(),   mSources.end());
    allSources.insert(mLibraries.begin(), mLibraries.end());
    allSources.insert(mCommands.begin(),  mCommands.end());

    return allSources;
}

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

ConfigSettings ProjectSettings::configSettings(const std::string& config) const
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

ConfigSettings& ProjectSettings::config(const std::string& config)
{
    return mConfigs[config];
}

void ProjectSettings::addConfig(const std::string& config)
{
    mConfigs.insert(std::make_pair(std::string(config), ConfigSettings()));
}

void ProjectSettings::removeConfig(const std::string& config)
{
    mConfigs.erase(config);
}

void ProjectSettings::copyConfig(const std::string& config, const std::string& newName)
{
    ConfigSettings settings = mConfigs[config];

    mConfigs.insert(std::make_pair(std::string(newName), std::move(settings)));
}

void ProjectSettings::renameConfig(const std::string& config, const std::string& newName)
{
    ConfigSettings settings = mConfigs[config];

    mConfigs.insert(std::make_pair(newName, std::move(settings)));

    mConfigs.erase(config);
}
