#ifndef BUILDSTEPLIST_H
#define BUILDSTEPLIST_H

#include <list>

#include "buildstep.h"

class BuildStepList
{
public:
    BuildStepList();
    BuildStepList(const BuildStepList& other);

    BuildStepList& operator=(const BuildStepList& other);
    bool operator==(const BuildStepList& other) const;
    bool operator!=(const BuildStepList& other) const;

    operator std::list<BuildStep>() const;
    std::list<BuildStep> get() const;

    void add(const BuildStep& buildStep);
    void add(const std::string& buildStep);
    void add(const std::string& action, int condition);

    void clear();

private:

    std::list<BuildStep> mBuildStepList;
};

#endif // BUILDSTEPLIST_H
