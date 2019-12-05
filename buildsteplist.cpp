#include "buildsteplist.h"

BuildStepList::BuildStepList()
{

}

BuildStepList::BuildStepList(const BuildStepList& other) :
    mBuildStepList(other.mBuildStepList)
{

}

BuildStepList& BuildStepList::operator=(const BuildStepList& other)
{
    this->mBuildStepList = other.mBuildStepList;

    return *this;
}

bool BuildStepList::operator==(const BuildStepList& other) const
{
    return (this->mBuildStepList == other.mBuildStepList);
}

bool BuildStepList::operator!=(const BuildStepList& other) const
{
    return !(*this == other);
}

BuildStepList::operator std::list<BuildStep>() const
{
    return mBuildStepList;
}

std::list<BuildStep> BuildStepList::get() const
{
    return mBuildStepList;
}

void BuildStepList::add(const BuildStep& buildStep)
{
    mBuildStepList.push_back(buildStep);
}

void BuildStepList::add(const std::string& buildStep)
{
    mBuildStepList.push_back(BuildStep::fromString(buildStep));
}

void BuildStepList::add(const std::string& action, int condition)
{
    mBuildStepList.push_back(BuildStep(action,
                                       BuildStep::BuildCondition(condition)));
}

void BuildStepList::clear()
{
    mBuildStepList.clear();
}
