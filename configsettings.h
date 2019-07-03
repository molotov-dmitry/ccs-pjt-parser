#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <set>
#include <map>
#include <list>

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

    stringlist preBuildSteps() const;
    stringlist postBuildSteps() const;

    void addPreBuildStep(const char* action);
    void addPostBuildStep(const char* action);

    //// Compiler options ======================================================

    stringlist defines() const;
    stringlist undefines() const;
    stringlist includePaths() const;
    stringlist compilerOptions() const;

    void addCompilerOption(const char* option);

    //// Linker options ========================================================

    stringlist linkerOptions() const;

    void addLinkerOption(const char* option);

    //// Archiver options ======================================================

    stringlist archiverOptions() const;

    void addArchiverOption(const char* option);

    //// Custom files compiler options =========================================

    stringsetmap fileOptionsAdded() const;
    stringsetmap fileOptionsRemoved() const;

    void addFileOptionAdded(const char* file, const char* option);
    void addFileOptionRemoved(const char* file, const char* option);

    //// Files linking order ===================================================

    void addFileLinkOrder(const char* file, uint order);

private:

    stringlist mPreBuildSteps;
    stringlist mPostBuildSteps;

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
