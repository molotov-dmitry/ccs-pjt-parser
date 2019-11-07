#include "fileoptions.h"

FileOptions::FileOptions() :
    mLinkOrder(-1),
    mExcludeFromBuild(false),
    mBuildCondition(BuildStep::IF_ANY_FILE_BUILDS)
{

}

bool FileOptions::isDefault()
{
    if (mLinkOrder >= 0)
    {
        return false;
    }

    if (mExcludeFromBuild)
    {
        return false;
    }

    if (mBuildCondition != BuildStep::IF_ANY_FILE_BUILDS)
    {
        return false;
    }

    if (not mOptionsAdded.empty() || not mOptionsRemoved.empty())
    {
        return false;
    }

    if (not mPreBuildSteps.get().empty() || not mPostBuildSteps.get().empty())
    {
        return false;
    }

    return true;
}

int FileOptions::linkOrder() const
{
    return mLinkOrder;
}

void FileOptions::setLinkOrder(unsigned int linkOrder)
{
    mLinkOrder = (int)linkOrder;
}

void FileOptions::removeLinkOrder()
{
    mLinkOrder = -1;
}

bool FileOptions::isExcludedFromBuild() const
{
    return mExcludeFromBuild;
}

void FileOptions::setExcludeFromBuild(bool excludeFromBuild)
{
    mExcludeFromBuild = excludeFromBuild;
}

int FileOptions::buildCondition() const
{
    return mBuildCondition;
}

void FileOptions::setBuildCondition(int buildCondition)
{
    mBuildCondition = (BuildStep::BuildCondition)buildCondition;
}

std::set<std::string> FileOptions::optionsAdded() const
{
    return mOptionsAdded;
}

std::set<std::string> FileOptions::optionsRemoved() const
{
    return mOptionsRemoved;
}

void FileOptions::addOptionAdded(const std::string& option)
{
    mOptionsAdded.insert(option);
}

void FileOptions::addOptionRemoved(const std::string& option)
{
    mOptionsRemoved.insert(option);
}

void FileOptions::removeOptionAdded(const std::string& option)
{
    mOptionsAdded.erase(option);
}

void FileOptions::removeOptionRemoved(const std::string& option)
{
    mOptionsRemoved.erase(option);
}

void FileOptions::clearOptionsAdded()
{
    mOptionsAdded.clear();
}

void FileOptions::clearOptionsRemoved()
{
    mOptionsRemoved.clear();
}

BuildStepList FileOptions::preBuildSteps() const
{
    return mPreBuildSteps;
}

BuildStepList&FileOptions::preBuildSteps()
{
    return mPreBuildSteps;
}

BuildStepList FileOptions::postBuildSteps() const
{
    return mPostBuildSteps;
}

BuildStepList&FileOptions::postBuildSteps()
{
    return mPostBuildSteps;
}
