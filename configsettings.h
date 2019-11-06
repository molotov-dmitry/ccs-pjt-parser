#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <set>
#include <map>
#include <list>

#include "buildsteplist.h"

typedef std::set<std::string> stringset;
typedef std::list<std::string> stringlist;

typedef const stringset cstringset;

typedef std::map<std::string, stringset> stringsetmap;
typedef std::map<std::string, stringlist> stringlistmap;

class ConfigSettings
{
public:

    ConfigSettings();

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

    stringsetmap fileOptionsAdded() const;
    stringsetmap fileOptionsRemoved() const;

    void addFileOptionAdded(const char* file, const char* option);
    void addFileOptionRemoved(const char* file, const char* option);

    void addFileOptionsAdded(const char* file, stringlist& options);
    void addFileOptionsRemoved(const char* file, stringlist& options);

    void removeFileOptionAdded(const char* file, const char* option);
    void removeFileOptionRemoved(const char* file, const char* option);

    void clearFileOptionAdded(const char* file);
    void clearFileOptionRemoved(const char* file);

    void clearFileOptionAdded();
    void clearFileOptionRemoved();

    //// Files linking order ===================================================

    std::map<std::string, uint> fileLinkOrder() const;

    void addFileLinkOrder(const char* file, uint order);

    void removeFileLinkOrder(const char* file);

    void clearFileLinkOrder();

private:

    BuildStepList mPreBuildSteps;
    BuildStepList mPostBuildSteps;

    stringlist mDefines;
    stringlist mUndefines;
    stringlist mIncludePaths;

    stringlist mCompilerOptions;
    stringlist mLinkerOptions;
    stringlist mArchiverOptions;

    stringsetmap mFileOptionsAdded;
    stringsetmap mFileOptionsRemoved;

    std::map<std::string, uint> mFileLinkOrder;
};

#endif // CONFIGSETTINGS_H
