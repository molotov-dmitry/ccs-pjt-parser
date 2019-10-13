#include "buildstep.h"

#include "utils.h"

static const std::string sRunAlwaysString = ";Run=Always";

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

    case BUILD_CONDITION_COUNT:
        return mCommand;
    }

    return mCommand;
}

BuildStep::operator std::pair<std::string, int>() const
{
    return std::pair<std::string, int>(mCommand, mCondition);
}
