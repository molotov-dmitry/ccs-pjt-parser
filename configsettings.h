﻿#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <set>
#include <map>
#include <list>

#include "buildsteplist.h"
#include "fileoptions.h"

typedef std::set<std::string> stringset;
typedef std::list<std::string> stringlist;

typedef const stringset cstringset;

typedef std::map<std::string, stringset> stringsetmap;
typedef std::map<std::string, stringlist> stringlistmap;

class ConfigSettings
{
public:

    ConfigSettings();
    ConfigSettings(const ConfigSettings& other);

    ConfigSettings& operator=(const ConfigSettings& other);
    bool operator==(const ConfigSettings& other) const;
    bool operator!=(const ConfigSettings& other) const;

    //// Build steps ===========================================================

    std::list<BuildStep> preBuildSteps() const;
    BuildStepList&       preBuildStepsRef();

    std::list<BuildStep> postBuildSteps() const;
    BuildStepList&       postBuildStepsRef();

    //// Compiler options ======================================================

    stringlist defines() const;
    stringlist undefines() const;
    stringlist includePaths() const;
    stringlist compilerOptions() const;

    void addCompilerOption(const char* option);

    void addDefine(const std::string& option);
    void addUndefine(const std::string& option);
    void addIncludePath(const std::string& option);
    void addOtherCompilerOption(const std::string& option);

    void addDefines(stringlist& options);
    void addUndefines(stringlist& options);
    void addIncludePaths(stringlist& options);
    void addCompilerOptions(stringlist& options);

    void removeDefine(const char* option);
    void removeUndefine(const char* option);
    void removeIncludePath(const char* option);
    void removeCompilerOption(const char* option);

    void clearDefines();
    void clearUndefines();
    void clearIncludePaths();
    void clearCompilerOptions();

    //// Linker options ========================================================

    stringlist linkerOptions() const;

    void addLinkerOption(const char* option);
    void addLinkerOption(const std::string& flag, const std::string& value, bool quote);
    void addLinkerOptions(stringlist& options);

    void removeLinkerOption(const char* option);

    void clearLinkerOptions();

    //// Archiver options ======================================================

    stringlist archiverOptions() const;

    void addArchiverOption(const char* option);
    void addArchiverOptions(stringlist& options);

    void removeArchiverOption(const char* option);

    void clearArchiverOptions();

    //// Custom files compiler options =========================================

    FileOptions fileOptions(const std::string& file) const;
    FileOptions& file(const std::string& file);

    void clearFileLinkOrder();

    //// Options getters =======================================================

    std::string compilerOption(const std::string& key, const std::string& defaultValue = std::string()) const;
    std::string linkerOption(const std::string& key, const std::string& defaultValue = std::string()) const;
    std::string archiverOption(const std::string& key, const std::string& defaultValue = std::string()) const;

private:

    BuildStepList mPreBuildSteps;
    BuildStepList mPostBuildSteps;

    stringlist mDefines;
    stringlist mUndefines;
    stringlist mIncludePaths;

    stringlist mCompilerOptions;
    stringlist mLinkerOptions;
    stringlist mArchiverOptions;

    std::map<std::string, FileOptions> mFileOptions;

    std::string getOption(const stringlist& options, const std::string& key, const std::string& defaultValue) const;

};

#endif // CONFIGSETTINGS_H
