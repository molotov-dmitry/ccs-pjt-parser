#ifndef FILEOPTIONS_H
#define FILEOPTIONS_H

#include <set>
#include <string>

#include "buildsteplist.h"

class FileOptions
{
public:
    FileOptions();
    FileOptions(const FileOptions& other);

    FileOptions& operator=(const FileOptions& other);
    bool operator==(const FileOptions& other) const;
    bool operator!=(const FileOptions& other) const;

    bool isDefault(bool considerLinkOrder = true);

    int linkOrder() const;
    void setLinkOrder(unsigned int linkOrder);
    void removeLinkOrder();

    bool isExcludedFromBuild() const;
    void setExcludeFromBuild(bool excludeFromBuild);

    int buildCondition() const;
    void setBuildCondition(int buildCondition);

    std::set<std::string> optionsAdded() const;
    std::set<std::string> optionsRemoved() const;

    void addOptionAdded(const std::string& option);
    void addOptionRemoved(const std::string& option);

    void removeOptionAdded(const std::string& option);
    void removeOptionRemoved(const std::string& option);

    void clearOptionsAdded();
    void clearOptionsRemoved();

    BuildStepList preBuildSteps() const;
    BuildStepList& preBuildSteps();

    BuildStepList postBuildSteps() const;
    BuildStepList& postBuildSteps();

private:

    int mLinkOrder;

    bool mExcludeFromBuild;

    BuildStep::BuildCondition mBuildCondition;

    std::set<std::string> mOptionsAdded;
    std::set<std::string> mOptionsRemoved;

    BuildStepList mPreBuildSteps;
    BuildStepList mPostBuildSteps;

};

#endif // FILEOPTIONS_H
