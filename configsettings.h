#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <set>
#include <map>
#include <list>

typedef std::set<std::string> stringset;
typedef std::list<std::string> stringlist;

typedef std::map<std::string, stringset> stringsetmap;
typedef std::map<std::string, stringlist> stringlistmap;

class ConfigSettings
{
public:

    ConfigSettings();

    void addPreBuildStep(const char* action);
    void addPostBuildStep(const char* action);

    void addCompilerOption(const char* option);
    void addLinkerOption(const char* option);
    void addArchiverOption(const char* option);

    void addFileOptionAdded(const char* file, const char* option);
    void addFileOptionRemoved(const char* file, const char* option);

    void addFileLinkOrder(const char* file, uint order);

    stringlist preBuildSteps() const;
    stringlist postBuildSteps() const;

    stringlist defines() const;
    stringlist includePaths() const;

    stringlist compilerOptions() const;
    stringlist linkerOptions() const;
    stringlist archiverOptions() const;

    stringsetmap fileOptionsAdded() const;
    stringsetmap fileOptionsRemoved() const;

private:

    stringlist mPreBuildSteps;
    stringlist mPostBuildSteps;

    stringlist mDefines;
    stringlist mIncludePaths;

    stringlist mCompilerOptions;
    stringlist mLinkerOptions;
    stringlist mArchiverOptions;

    stringsetmap mFileOptionsAdded;
    stringsetmap mFileOptionsRemoved;

    std::map<std::string, uint> mFileLinkOrder;
};

#endif // CONFIGSETTINGS_H
