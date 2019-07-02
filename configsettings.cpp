﻿#include "configsettings.h"

#include <string.h>

#include "utils.h"

ConfigSettings::ConfigSettings()
{

}

void ConfigSettings::addPreBuildStep(const char *action)
{
    mPreBuildSteps.push_back(action);
}

void ConfigSettings::addPostBuildStep(const char *action)
{
    mPostBuildSteps.push_back(action);
}

void ConfigSettings::addCompilerOption(const char *option)
{
    if (starts_with(option, "-i\"") && ends_with(option, "\""))
    {
        std::string includePath = std::string(option).substr(3, strlen(option) - 4);

        mIncludePaths.push_back(includePath);
    }
    else if (starts_with(option, "-d\"") && ends_with(option, "\""))
    {
        std::string define = std::string(option).substr(3, strlen(option) - 4);

        mDefines.push_back(define);
    }
    else
    {
        std::string opt = std::string(option);

        mCompilerOptions.push_back(opt);
    }
}

void ConfigSettings::addLinkerOption(const char *option)
{
    std::string opt = std::string(option);

    mLinkerOptions.push_back(opt);
}

void ConfigSettings::addArchiverOption(const char *option)
{
    std::string opt = std::string(option);

    mArchiverOptions.push_back(opt);
}

void ConfigSettings::addFileOptionAdded(const char *file, const char *option)
{
    stringset& opt = mFileOptionsAdded[file];

    opt.insert(option);
}

void ConfigSettings::addFileOptionRemoved(const char *file, const char *option)
{
    stringset& opt = mFileOptionsRemoved[file];

    opt.insert(option);
}

void ConfigSettings::addFileLinkOrder(const char *file, uint order)
{
    mFileLinkOrder[file] = order;
}

stringlist ConfigSettings::preBuildSteps() const
{
    return mPreBuildSteps;
}

stringlist ConfigSettings::postBuildSteps() const
{
    return mPostBuildSteps;
}

stringlist ConfigSettings::defines() const
{
    return mDefines;
}

stringlist ConfigSettings::includePaths() const
{
    return mIncludePaths;
}

stringlist ConfigSettings::compilerOptions() const
{
    return mCompilerOptions;
}

stringlist ConfigSettings::linkerOptions() const
{
    return mLinkerOptions;
}

stringlist ConfigSettings::archiverOptions() const
{
    return mArchiverOptions;
}

stringsetmap ConfigSettings::fileOptionsAdded() const
{
    return mFileOptionsAdded;
}

stringsetmap ConfigSettings::fileOptionsRemoved() const
{
    return mFileOptionsRemoved;
}
