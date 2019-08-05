#ifndef ABSTRACTPROJECTEXPORT_H
#define ABSTRACTPROJECTEXPORT_H

#include <string>
#include <fstream>

#include "projectsettings.h"

class AbstractProjectExport
{
public:
    AbstractProjectExport();
    virtual ~AbstractProjectExport();

    bool write(const ProjectSettings& settings, const char* path = nullptr);

    std::string lastError() const;

protected:
    std::string mLastError;

    virtual bool writeData(const ProjectSettings& settings, std::ostream& file) = 0;

private:
    std::string mPath;
};

#endif // ABSTRACTPROJECTEXPORT_H
