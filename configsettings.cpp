#include "configsettings.h"

#include <string.h>

#include "utils.h"

ConfigSettings::ConfigSettings()
{

}

//// Build steps ===============================================================

std::list<BuildStep> ConfigSettings::preBuildSteps() const
{
    return mPreBuildSteps.get();
}

BuildStepList& ConfigSettings::preBuildStepsRef()
{
    return mPreBuildSteps;
}

std::list<BuildStep> ConfigSettings::postBuildSteps() const
{
    return mPostBuildSteps.get();
}

BuildStepList& ConfigSettings::postBuildStepsRef()
{
    return mPostBuildSteps;
}

//// Compiler options ==========================================================

stringlist ConfigSettings::defines() const
{
    return mDefines;
}

stringlist ConfigSettings::undefines() const
{
    return mUndefines;
}

stringlist ConfigSettings::includePaths() const
{
    return mIncludePaths;
}

stringlist ConfigSettings::compilerOptions() const
{
    return mCompilerOptions;
}

void ConfigSettings::addCompilerOption(const char* option)
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
    else if (starts_with(option, "-u\"") && ends_with(option, "\""))
    {
        std::string undefine = std::string(option).substr(3, strlen(option) - 4);

        mUndefines.push_back(undefine);
    }
    else
    {
        std::string opt = std::string(option);

        mCompilerOptions.push_back(opt);
    }
}

void ConfigSettings::addDefine(const std::string& option)
{
    mDefines.push_back(option);
}

void ConfigSettings::addUndefine(const std::string& option)
{
    mUndefines.push_back(option);
}

void ConfigSettings::addIncludePath(const std::string& option)
{
    mIncludePaths.push_back(option);
}

void ConfigSettings::addOtherCompilerOption(const std::string& option)
{
    mCompilerOptions.push_back(option);
}

void ConfigSettings::addDefines(stringlist& options)
{
    mDefines.splice(mDefines.end(), options);
}

void ConfigSettings::addUndefines(stringlist& options)
{
    mUndefines.splice(mUndefines.end(), options);
}

void ConfigSettings::addIncludePaths(stringlist& options)
{
    mIncludePaths.splice(mIncludePaths.end(), options);
}

void ConfigSettings::addCompilerOptions(stringlist& options)
{
    mCompilerOptions.splice(mCompilerOptions.end(), options);
}

void ConfigSettings::removeDefine(const char* option)
{
    mDefines.remove(option);
}

void ConfigSettings::removeUndefine(const char* option)
{
    mUndefines.remove(option);
}

void ConfigSettings::removeIncludePath(const char* option)
{
    mIncludePaths.remove(option);
}

void ConfigSettings::removeCompilerOption(const char* option)
{
    mCompilerOptions.remove(option);
}

void ConfigSettings::clearDefines()
{
    mDefines.clear();
}

void ConfigSettings::clearUndefines()
{
    mUndefines.clear();
}

void ConfigSettings::clearIncludePaths()
{
    mIncludePaths.clear();
}

void ConfigSettings::clearCompilerOptions()
{
    mCompilerOptions.clear();
}

//// Linker options ============================================================

stringlist ConfigSettings::linkerOptions() const
{
    return mLinkerOptions;
}

void ConfigSettings::addLinkerOption(const char* option)
{
    mLinkerOptions.push_back(option);
}

void ConfigSettings::addLinkerOptions(stringlist& options)
{
    mLinkerOptions.splice(mLinkerOptions.end(), options);
}

void ConfigSettings::removeLinkerOption(const char* option)
{
    mLinkerOptions.remove(option);
}

void ConfigSettings::clearLinkerOptions()
{
    mLinkerOptions.clear();
}

//// Archiver options ==========================================================

stringlist ConfigSettings::archiverOptions() const
{
    return mArchiverOptions;
}

void ConfigSettings::addArchiverOption(const char* option)
{
    mArchiverOptions.push_back(option);
}

void ConfigSettings::addArchiverOptions(stringlist& options)
{
    mArchiverOptions.splice(mArchiverOptions.end(), options);
}

void ConfigSettings::removeArchiverOption(const char* option)
{
    mArchiverOptions.remove(option);
}

void ConfigSettings::clearArchiverOptions()
{
    mArchiverOptions.clear();
}

//// Custom files compiler options =============================================

FileOptions ConfigSettings::fileOptions(const std::string& file)
{
    if (mFileOptions.find(file) != mFileOptions.end())
    {
        return mFileOptions.at(file);
    }
    else
    {
        return FileOptions();
    }
}

FileOptions&ConfigSettings::file(const std::string& file)
{
    return mFileOptions[file];
}

void ConfigSettings::clearFileLinkOrder()
{
    for (auto& file : mFileOptions)
    {
        file.second.removeLinkOrder();
    }
}


