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
    mLibraryPaths(other.mLibraryPaths),
    mLibraries(other.mLibraries),
    mOtherCompilerOptions(other.mOtherCompilerOptions),
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
    this->mLibraryPaths = other.mLibraryPaths;
    this->mLibraries = other.mLibraries;
    this->mOtherCompilerOptions = other.mOtherCompilerOptions;
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

    if (this->mLibraryPaths != other.mLibraryPaths)
    {
        return false;
    }

    if (this->mLibraries != other.mLibraries)
    {
        return false;
    }

    if (this->mOtherCompilerOptions != other.mOtherCompilerOptions)
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
    return mOtherCompilerOptions;
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
        mOtherCompilerOptions.push_back(option);
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
        mOtherCompilerOptions.remove(option);
    }
}

void ConfigSettings::clearCompilerOptions()
{
    mIncludePaths.clear();
    mDefines.clear();
    mUndefines.clear();
    mOtherCompilerOptions.clear();
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
    mOtherCompilerOptions.push_back(option);
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
    mOtherCompilerOptions.remove(option);
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
    mOtherCompilerOptions.clear();
}

//// Linker options ============================================================

stringlist ConfigSettings::libraryPaths() const
{
    return mLibraryPaths;
}

stringlist ConfigSettings::libraries() const
{
    return mLibraries;
}

stringlist ConfigSettings::otherLinkerOptions() const
{
    return mOtherLinkerOptions;
}

void ConfigSettings::addLinkerOption(const std::string& option)
{
    std::string value;

    if (is_flag(option, "-i", value))
    {
        mLibraryPaths.push_back(fixpath(value));
    }
    else if (is_flag(option, "-l", value))
    {
        mLibraries.push_back(fixpath(value));
    }
    else
    {
        mOtherLinkerOptions.push_back(option);
    }
}

void ConfigSettings::addLinkerOption(const std::string& flag, const std::string& value, bool quote)
{
    if (flag == "-i")
    {
        mLibraryPaths.push_back(value);
    }
    else if (flag == "-l")
    {
        mLibraries.push_back(value);
    }
    else
    {
        mOtherLinkerOptions.push_back(to_option(flag, value, quote));
    }
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
    std::string value;

    if (is_flag(option, "-i", value))
    {
        mLibraryPaths.remove(fixpath(value));
    }
    else if (is_flag(option, "-l", value))
    {
        mLibraries.remove(fixpath(value));
    }
    else
    {
        mOtherLinkerOptions.remove(option);
    }
}

void ConfigSettings::clearLinkerOptions()
{
    mLibraryPaths.clear();
    mLibraries.clear();
    mOtherLinkerOptions.clear();
}

void ConfigSettings::addLibraryPath(const std::string& option)
{
    mLibraryPaths.push_back(fixpath(option));
}

void ConfigSettings::addLibrary(const std::string& option)
{
    mLibraries.push_back(fixpath(option));
}

void ConfigSettings::addOtherLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.push_back(option);
}

void ConfigSettings::addLibraryPaths(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addLibraryPath(option);
    }
}

void ConfigSettings::addLibraries(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addLibrary(option);
    }
}

void ConfigSettings::addOtherLinkerOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addOtherLinkerOption(option);
    }
}

void ConfigSettings::removeLibraryPath(const std::string& option)
{
    mLibraryPaths.remove(fixpath(option));
}

void ConfigSettings::removeLibrary(const std::string& option)
{
    mLibraries.remove(fixpath(option));
}

void ConfigSettings::removeOtherLinkerOption(const std::string& option)
{
    mOtherLinkerOptions.remove(option);
}

void ConfigSettings::clearLibraryPaths()
{
    mLibraryPaths.clear();
}

void ConfigSettings::clearLibraries()
{
    mLibraries.clear();
}

void ConfigSettings::clearOtherLinkerOptions()
{
    mOtherLinkerOptions.clear();
}

//// Archiver options ==========================================================

stringlist ConfigSettings::otherArchiverOptions() const
{
    return mOtherArchiverOptions;
}

void ConfigSettings::addArchiverOption(const char* option)
{
    mOtherArchiverOptions.push_back(option);
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
    mOtherArchiverOptions.remove(option);
}

void ConfigSettings::clearArchiverOptions()
{
    mOtherArchiverOptions.clear();
}

void ConfigSettings::addOtherArchiverOption(const std::string& option)
{
    mOtherArchiverOptions.push_back(option);
}

void ConfigSettings::addOtherArchiverOptions(const stringlist& options)
{
    for (const std::string& option : options)
    {
        addOtherArchiverOption(option);
    }
}

void ConfigSettings::removeOtherArchiverOption(const std::string& option)
{
    mOtherArchiverOptions.remove(option);
}

void ConfigSettings::clearOtherArchiverOptions()
{
    mOtherArchiverOptions.clear();
}

//// Custom files compiler options =============================================

FileOptions ConfigSettings::fileOptions(const std::string& file) const
{
    std::string fileFixed = fixpath(file);

    if (mFileOptions.find(fileFixed) != mFileOptions.end())
    {
        return mFileOptions.at(fileFixed);
    }
    else
    {
        return FileOptions();
    }
}

FileOptions&ConfigSettings::file(const std::string& file)
{
    return mFileOptions[fixpath(file)];
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
    return getOption(mOtherCompilerOptions, key, defaultValue);
}

std::string ConfigSettings::linkerOption(const std::string& key, const std::string& defaultValue) const
{
    return getOption(mOtherLinkerOptions, key, defaultValue);
}

std::string ConfigSettings::archiverOption(const std::string& key, const std::string& defaultValue) const
{
    return getOption(mOtherArchiverOptions, key, defaultValue);
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


