#ifndef BUILDSTEP_H
#define BUILDSTEP_H

#include <string>

class BuildStep
{
public:
    enum BuildCondition
    {
        IF_ANY_FILE_BUILDS,
        ALWAYS,
        NEVER,

        BUILD_CONDITION_COUNT
    };

public:
    BuildStep();
    BuildStep(const std::string& command, BuildCondition condition);

    operator std::pair<std::string, int>() const;
    bool operator==(const BuildStep& other) const;

    std::string command() const;
    int         condition() const;

    static BuildStep fromString(std::string command);
    std::string toString() const;

    static std::string buildConditionString(int condition, bool file);

private:

    std::string    mCommand;
    BuildCondition mCondition;

};

#endif // BUILDSTEP_H
