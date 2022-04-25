#ifndef PROJECTEXPORTQTMAKEFILE_H
#define PROJECTEXPORTQTMAKEFILE_H

#include "abstractprojectexport.h"

class ProjectExportQtMakefileSources : public AbstractProjectExport
{
public:
    explicit ProjectExportQtMakefileSources();

    bool writeData(const ProjectSettings& settings, std::ostream& file) override;

private:

    std::string mConfig;
};

class ProjectExportQtMakefileDefines : public AbstractProjectExport
{
public:
    explicit ProjectExportQtMakefileDefines(const std::string& config);

    bool writeData(const ProjectSettings& settings, std::ostream& file) override;

private:

    std::string mConfig;
};

class ProjectExportQtMakefileIncludes : public AbstractProjectExport
{
public:
    explicit ProjectExportQtMakefileIncludes(const std::string& config);

    bool writeData(const ProjectSettings& settings, std::ostream& file) override;

private:

    std::string mConfig;
};

#endif // PROJECTEXPORTQTMAKEFILE_H
