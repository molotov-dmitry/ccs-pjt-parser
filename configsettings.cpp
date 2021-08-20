#include "configsettings.h"

#include <string.h>

#include "utils.h"

ConfigSettings::ConfigSettings()
{

}

ConfigSettings::ConfigSettings(const ConfigSettings& other) :
    mPreBuildSteps(other.mPreBuildSteps),
    mPostBuildSteps(other.mPostBuildSteps),
    mDefines(other.mDefines),
    mUndefines(other.mUndefines),
    mIncludePaths(other.mIncludePaths),
    mCompilerOptions(other.mCompilerOptions),
    mLinkerOptions(other.mLinkerOptions),
    mArchiverOptions(other.mArchiverOptions),
    mFileOptions(other.mFileOptions)
{

}

ConfigSettings&ConfigSettings::operator=(const ConfigSettings& other)
{
    this->mPreBuildSteps = other.mPreBuildSteps;
    this->mPostBuildSteps = other.mPostBuildSteps;
    this->mDefines = other.mDefines;
    this->mUndefines = other.mUndefines;
    this->mIncludePaths = other.mIncludePaths;
    this->mCompilerOptions = other.mCompilerOptions;
    this->mLinkerOptions = other.mLinkerOptions;
    this->mArchiverOptions = other.mArchiverOptions;
    this->mFileOptions = other.mFileOptions;

    return *this;
}

bool ConfigSettings::operator==(const ConfigSettings& other) const
{
    if (this->mPreBuildSteps != other.mPreBuildSteps)
    {
        return false;
    }

    if (this->mPostBuildSteps != other.mPostBuildSteps)
    {
        return false;
    }

    if (this->mDefines != other.mDefines)
    {
        return false;
    }

    if (this->mUndefines != other.mUndefines)
    {
        return false;
    }

    if (this->mIncludePaths != other.mIncludePaths)
    {
        return false;
    }

    if (this->mCompilerOptions != other.mCompilerOptions)
    {
        return false;
    }

    if (this->mLinkerOptions != other.mLinkerOptions)
    {
        return false;
    }

    if (this->mArchiverOptions != other.mArchiverOptions)
    {
        return false;
    }

    if (this->mFileOptions != other.mFileOptions)
    {
        return false;
    }

    return true;
}

bool ConfigSettings::operator!=(const ConfigSettings& other) const
{
    return !(*this == other);
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
        std::string includePath = fixpath(std::string(option).substr(3, strlen(option) - 4));

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
    mIncludePaths.push_back(fixpath(option));
}

void ConfigSettings::addOtherCompilerOption(const std::string& option)
{
    mCompilerOptions.push_back(option);
}

void ConfigSettings::addDefines(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addDefine(option);
    }
}

void ConfigSettings::addUndefines(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addUndefine(option);
    }
}

void ConfigSettings::addIncludePaths(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addIncludePath(option);
    }
}

void ConfigSettings::addCompilerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addOtherCompilerOption(option.c_str());
    }
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

void ConfigSettings::addLinkerOption(const std::string& flag, const std::string& value, bool quote)
{
    mLinkerOptions.push_back(to_option(flag, value, quote));
}

void ConfigSettings::addLinkerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addLinkerOption(option.c_str());
    }
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

void ConfigSettings::addArchiverOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addArchiverOption(option.c_str());
    }
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

FileOptions ConfigSettings::fileOptions(const std::string& file) const
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

std::string ConfigSettings::compilerOption(const std::string& key, const std::string& defaultValue) const
{
    return getOption(mCompilerOptions, key, defaultValue);
}

std::string ConfigSettings::linkerOption(const std::string& key, const std::string& defaultValue) const
{
    return getOption(mLinkerOptions, key, defaultValue);
}

std::string ConfigSettings::archiverOption(const std::string& key, const std::string& defaultValue) const
{
    return getOption(mArchiverOptions, key, defaultValue);
}

std::string ConfigSettings::getOption(const stringlist& options, const std::string& key, const std::string& defaultValue) const
{
    for (const std::string& option : options)
    {
        std::string value;

        if (not starts_with(option, key, true))
        {
            continue;
        }

        std::string::size_type optionLen = key.length();
        if ((option.length() > optionLen) && (option.at(optionLen) == '='))
        {
            value = option.data() + optionLen + 1;
        }
        else
        {
            value = option.data() + optionLen;
        }

        remove_quotes(value);

        return value;
    }

    return defaultValue;
}


