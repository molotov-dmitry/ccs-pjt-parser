#ifndef PROJECTEXPORTMAKEFILE_H
#define PROJECTEXPORTMAKEFILE_H

#include "abstractprojectexport.h"

class ProjectExportMakefile : public AbstractProjectExport
{
public:
    ProjectExportMakefile();
    void setTarget(std::string target);
    void setTabWidth(size_t tabWidth);

private:

    std::string mTarget;
    size_t      mTabWidth;

    virtual bool writeData(const ProjectSettings& settings, std::ostream& out);

    void writeConfig(std::ostream &out, const char* name, const std::string &value, bool constant = true);
    void writeConfig(std::ostream &out, const char* name, const std::string &nameSuffix, const std::string &value, bool constant = true);

    void writeComment(std::ostream &out, size_t level, const std::string &name, bool emptyLine = true);
};

#endif // PROJECTEXPORTMAKEFILE_H
