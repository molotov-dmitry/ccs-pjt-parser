#include "projectexportqtmakefile.h"

ProjectExportQtMakefileSources::ProjectExportQtMakefileSources()
{

}

bool ProjectExportQtMakefileSources::writeData(const ProjectSettings& settings, std::ostream& file)
{
    for (const std::string& source : settings.c_sources())
    {
        file << source << std::endl;
    }

    return true;
}

ProjectExportQtMakefileDefines::ProjectExportQtMakefileDefines(const std::string& config) :
    mConfig(config)
{

}

bool ProjectExportQtMakefileDefines::writeData(const ProjectSettings& settings, std::ostream& file)
{
    const ConfigSettings& config = settings.configSettings(mConfig);

    for (std::string define : config.defines())
    {
        file << "#define " << define << std::endl;
    }

    for (std::string undefine : config.undefines())
    {
        file << "#undef " << undefine << std::endl;
    }

    return true;
}

ProjectExportQtMakefileIncludes::ProjectExportQtMakefileIncludes(const std::string& config) :
    mConfig(config)
{

}

bool ProjectExportQtMakefileIncludes::writeData(const ProjectSettings& settings, std::ostream& file)
{
    const ConfigSettings& config = settings.configSettings(mConfig);

    for (std::string include : config.includePaths())
    {
        file << include << std::endl;
    }

    return true;
}
