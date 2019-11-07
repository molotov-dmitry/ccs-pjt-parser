#include "buildstep.h"

#include "utils.h"

static const std::string sRunIfAnyFileBuildsValue = "If any file builds";
static const std::string sRunIfFileBuildsValue    = "If file builds";
static const std::string sRunAlwaysValue          = "Always";
static const std::string sRunNeverValue           = "Never";

static const std::string sRunAlwaysString = ";Run=" + sRunAlwaysValue;
static const std::string sRunNeverString = ";Run=" + sRunNeverValue;

BuildStep::BuildStep() : mCondition(IF_ANY_FILE_BUILDS)
{

}

BuildStep::BuildStep(const std::string &command, BuildCondition condition) :
    mCommand(command), mCondition(condition)
{

}

bool BuildStep::operator==(const BuildStep& other) const
{
    return (this->mCommand == other.mCommand &&
            this->mCondition == other.mCondition);
}

std::string BuildStep::command() const
{
    return mCommand;
}

int BuildStep::condition() const
{
    return mCondition;
}

BuildStep BuildStep::fromString(std::string command)
{
    BuildCondition condition = IF_ANY_FILE_BUILDS;

    if (ends_with(command, sRunAlwaysString, false))
    {
        command.resize(command.size() - sRunAlwaysString.size());
        condition = ALWAYS;
    }
    else if (ends_with(command, sRunNeverString, false))
    {
        command.resize(command.size() - sRunNeverString.size());
        condition = NEVER;
    }

    return BuildStep(command, condition);
}

std::string BuildStep::toString() const
{
    switch (mCondition)
    {
    case IF_ANY_FILE_BUILDS:
        return mCommand;

    case ALWAYS:
        return mCommand + sRunAlwaysString;

    case NEVER:
        return mCommand + sRunNeverString;

    case BUILD_CONDITION_COUNT:
        return mCommand;
    }

    return mCommand;
}

std::string BuildStep::buildConditionString(int condition, bool file)
{
    switch (condition)
    {
    case IF_ANY_FILE_BUILDS:
        return file ? sRunIfFileBuildsValue : sRunIfAnyFileBuildsValue;

    case ALWAYS:
        return sRunAlwaysValue;

    case NEVER:
        return sRunNeverValue;

    case BUILD_CONDITION_COUNT:
        return std::string();
    }
}

BuildStep::operator std::pair<std::string, int>() const
{
    return std::pair<std::string, int>(mCommand, mCondition);
}
