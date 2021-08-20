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
    mOtherLinkerOptions(other.mOtherLinkerOptions),
    mOtherArchiverOptions(other.mOtherArchiverOptions),
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
    this->mOtherLinkerOptions = other.mOtherLinkerOptions;
    this->mOtherArchiverOptions = other.mOtherArchiverOptions;
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

    if (this->mOtherLinkerOptions != other.mOtherLinkerOptions)
    {
        return false;
    }

    if (this->mOtherArchiverOptions != other.mOtherArchiverOptions)
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

stringlist ConfigSettings::otherCompilerOptions() const
{
    return mCompilerOptions;
}

void ConfigSettings::addCompilerOption(const std::string& option)
{
    std::string value;

    if (is_flag(option, "-i", value))
    {
        mIncludePaths.push_back(fixpath(value));
    }
    else if (is_flag(option, "-d", value))
    {
        mDefines.push_back(value);
    }
    else if (is_flag(option, "-u", value))
    {
        mUndefines.push_back(value);
    }
    else
    {
        mCompilerOptions.push_back(option);
    }
}

void ConfigSettings::addCompilerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addCompilerOption(option);
    }
}

void ConfigSettings::removeCompilerOption(const std::string& option)
{
    std::string value;

    if (is_flag(option, "-i", value))
    {
        mIncludePaths.remove(fixpath(value));
    }
    else if (is_flag(option, "-d", value))
    {
        mDefines.remove(value);
    }
    else if (is_flag(option, "-u", value))
    {
        mUndefines.remove(value);
    }
    else
    {
        mCompilerOptions.remove(option);
    }
}

void ConfigSettings::clearCompilerOptions()
{
    mIncludePaths.clear();
    mDefines.clear();
    mUndefines.clear();
    mCompilerOptions.clear();
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

void ConfigSettings::addOtherCompilerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addOtherCompilerOption(option);
    }
}

void ConfigSettings::removeDefine(const std::string& option)
{
    mDefines.remove(option);
}

void ConfigSettings::removeUndefine(const std::string& option)
{
    mUndefines.remove(option);
}

void ConfigSettings::removeIncludePath(const std::string& option)
{
    mIncludePaths.remove(fixpath(option));
}

void ConfigSettings::removeOtherCompilerOption(const std::string& option)
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

void ConfigSettings::clearOtherCompilerOptions()
{
    mCompilerOptions.clear();
}

//// Linker options ============================================================

stringlist ConfigSettings::otherLinkerOptions() const
{
    return mOtherLinkerOptions;
}

void ConfigSettings::addLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.push_back(option);
}

void ConfigSettings::addLinkerOption(const std::string& flag, const std::string& value, bool quote)
{
    mOtherLinkerOptions.push_back(to_option(flag, value, quote));
}

void ConfigSettings::addLinkerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addLinkerOption(option.c_str());
    }
}

void ConfigSettings::removeLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.remove(option);
}

void ConfigSettings::clearLinkerOptions()
{
    mOtherLinkerOptions.clear();
}

void ConfigSettings::addOtherLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.push_back(option);
}

void ConfigSettings::addOtherLinkerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addOtherLinkerOption(option);
    }
}

void ConfigSettings::removeOtherLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.remove(option);
}

void ConfigSettings::clearOtherLinkerOptions()
{
    mOtherLinkerOptions.clear();
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
    return getOption(mOtherLinkerOptions, key, defaultValue);
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


