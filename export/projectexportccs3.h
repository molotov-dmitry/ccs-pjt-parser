#ifndef PROJECTEXPORTCCS3_H
#define PROJECTEXPORTCCS3_H

#include "abstractprojectexport.h"

class ProjectExportCcs3 : public AbstractProjectExport
{
public:
    ProjectExportCcs3();

private:
    virtual bool writeData(const ProjectSettings& settings, std::ostream& out);

    void writeConfig(std::ostream &out, const char* name, const std::string &value, bool quote = true);
};

#endif // PROJECTEXPORTCCS3_H
