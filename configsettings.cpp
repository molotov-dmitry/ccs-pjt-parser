#include "configsettings.h"

#include <string.h>

#include "utils.h"

ConfigSettings::ConfigSettings()
{

}

//// Build steps ===============================================================

stringlist ConfigSettings::preBuildSteps() const
{
    return mPreBuildSteps;
}

stringlist ConfigSettings::postBuildSteps() const
{
    return mPostBuildSteps;
}

void ConfigSettings::addPreBuildStep(const char* action)
{
    mPreBuildSteps.push_back(action);
}

void ConfigSettings::addPostBuildStep(const char* action)
{
    mPostBuildSteps.push_back(action);
}

void ConfigSettings::addPreBuildSteps(stringlist& actions)
{
    mPreBuildSteps.splice(mPreBuildSteps.end(), actions);
}

void ConfigSettings::addPostBuildSteps(stringlist& actions)
{
    mPostBuildSteps.splice(mPostBuildSteps.end(), actions);
}

void ConfigSettings::removePreBuildStep(const char* action)
{
    mPreBuildSteps.remove(std::string(action));
}

void ConfigSettings::removePostBuildStep(const char* action)
{
    mPostBuildSteps.remove(std::string(action));
}

void ConfigSettings::clearPreBuildSteps()
{
    mPreBuildSteps.clear();
}

void ConfigSettings::clearPostBuildSteps()
{
    mPostBuildSteps.clear();
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

stringsetmap ConfigSettings::fileOptionsAdded() const
{
    return mFileOptionsAdded;
}

stringsetmap ConfigSettings::fileOptionsRemoved() const
{
    return mFileOptionsRemoved;
}

void ConfigSettings::addFileOptionAdded(const char* file, const char* option)
{
    stringset& opt = mFileOptionsAdded[file];

    opt.insert(option);
}

void ConfigSettings::addFileOptionRemoved(const char* file, const char* option)
{
    stringset& opt = mFileOptionsRemoved[file];

    opt.insert(option);
}

void ConfigSettings::addFileOptionsAdded(const char* file, stringlist& options)
{
    stringset& opt = mFileOptionsAdded[file];

    opt.insert(options.begin(), options.end());
}

void ConfigSettings::addFileOptionsRemoved(const char* file, stringlist& options)
{
    stringset& opt = mFileOptionsRemoved[file];

    opt.insert(options.begin(), options.end());
}

void ConfigSettings::removeFileOptionAdded(const char* file, const char* option)
{
    stringset& opt = mFileOptionsAdded[file];

    opt.erase(option);
}

void ConfigSettings::removeFileOptionRemoved(const char* file, const char* option)
{
    stringset& opt = mFileOptionsRemoved[file];

    opt.erase(option);
}

void ConfigSettings::clearFileOptionAdded(const char* file)
{
    mFileOptionsAdded.erase(file);
}

void ConfigSettings::clearFileOptionRemoved(const char* file)
{
    mFileOptionsRemoved.erase(file);
}

void ConfigSettings::clearFileOptionAdded()
{
    mFileOptionsAdded.clear();
}

void ConfigSettings::clearFileOptionRemoved()
{
    mFileOptionsRemoved.clear();
}

//// Files linking order =======================================================

std::map<std::string, uint> ConfigSettings::fileLinkOrder() const
{
    return mFileLinkOrder;
}

void ConfigSettings::addFileLinkOrder(const char* file, uint order)
{
    mFileLinkOrder[file] = order;
}

void ConfigSettings::removeFileLinkOrder(const char* file)
{
    mFileLinkOrder.erase(file);
}

void ConfigSettings::clearFileLinkOrder()
{
    mFileLinkOrder.clear();
}

