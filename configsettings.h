#ifndef CONFIGSETTINGS_H
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
    stringlist otherCompilerOptions() const;

    void addCompilerOption(const std::string& option);
    void addCompilerOptions(const stringlist& options);
    void removeCompilerOption(const std::string& option);
    void clearCompilerOptions();

    void addDefine(const std::string& option);
    void addUndefine(const std::string& option);
    void addIncludePath(const std::string& option);
    void addOtherCompilerOption(const std::string& option);

    void addDefines(const stringlist& options);
    void addUndefines(const stringlist& options);
    void addIncludePaths(const stringlist& options);
    void addOtherCompilerOptions(const stringlist& options);

    void removeDefine(const std::string& option);
    void removeUndefine(const std::string& option);
    void removeIncludePath(const std::string& option);
    void removeOtherCompilerOption(const std::string& option);

    void clearDefines();
    void clearUndefines();
    void clearIncludePaths();
    void clearOtherCompilerOptions();

    //// Linker options ========================================================

    stringlist otherLinkerOptions() const;

    void addLinkerOption(const std::string& option);
    void addLinkerOption(const std::string& flag, const std::string& value, bool quote);
    void addLinkerOptions(const stringlist& options);
    void removeLinkerOption(const std::string& option);
    void clearLinkerOptions();

    void addOtherLinkerOption(const std::string& option);

    void addOtherLinkerOptions(const stringlist& options);

    void removeOtherLinkerOption(const std::string& option);

    void clearOtherLinkerOptions();

    //// Archiver options ======================================================

    stringlist otherArchiverOptions() const;

    void addArchiverOption(const char* option);
    void addArchiverOptions(const stringlist& options);
    void removeArchiverOption(const char* option);
    void clearArchiverOptions();

    void addOtherArchiverOption(const std::string& option);

    void addOtherArchiverOptions(const stringlist& options);

    void removeOtherArchiverOption(const std::string& option);

    void clearOtherArchiverOptions();

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
    stringlist mOtherLinkerOptions;
    stringlist mOtherArchiverOptions;

    std::map<std::string, FileOptions> mFileOptions;

    std::string getOption(const stringlist& options, const std::string& key, const std::string& defaultValue) const;

};

#endif // CONFIGSETTINGS_H
