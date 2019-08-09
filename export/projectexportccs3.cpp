#include "projectexportccs3.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "utils.h"

ProjectExportCcs3::ProjectExportCcs3() : AbstractProjectExport ()
{

}

bool ProjectExportCcs3::writeData(const ProjectSettings &settings, std::ostream &out)
{
    //// Header ================================================================

    out << "; Code Composer Project File, Version 2.0 (do not modify or remove this line)" << std::endl;
    out << std::endl;

    //// Project settings ======================================================

    out << "[Project Settings]" << std::endl;

    writeConfig(out, "ProjectDir", settings.projectDir());
    writeConfig(out, "ProjectType", settings.projectTypeString(), false);
    writeConfig(out, "CPUFamily", settings.cpuFamily(), false);

    for (const std::string& tool : settings.c_tools())
    {
        writeConfig(out, "Tool", tool);
    }

    for (const std::string& config : settings.configs())
    {
        writeConfig(out, "Config", config);
    }

    out << std::endl;

    //// Source files ==========================================================

    out << "[Source Files]" << std::endl;

    for (const std::string& source : settings.c_libraries())
    {
        writeConfig(out, "Source", source);
    }

    for (const std::string& source : settings.c_sources())
    {
        writeConfig(out, "Source", source);
    }

    for (const std::string& source : settings.c_commands())
    {
        writeConfig(out, "Source", source);
    }

    out << std::endl;

    //// Config settings =======================================================

    for (const std::string& configName : settings.configs())
    {
        out << "[\"" << configName << "\" Settings]" << std::endl;

        ConfigSettings config = settings.configSettings(configName.c_str());

        for (const std::string& step : config.preBuildSteps())
        {
            writeConfig(out, "InitialBuildCmd", step, false);
        }

        for (const std::string& step : config.postBuildSteps())
        {
            writeConfig(out, "FinalBuildCmd", step, false);
        }

        out << std::endl;
    }

    //// Compiler options ======================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_COMPILER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName.c_str());

            out << "[\"Compiler\" Settings: \"" << configName << "\"]" << std::endl;

            writeConfig(out, "Options", join(config.compilerOptions(), ' '), false);

            out << std::endl;
        }
    }

    //// Compiler options ======================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_LINKER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName.c_str());

            out << "[\"Linker\" Settings: \"" << configName << "\"]" << std::endl;

            writeConfig(out, "Options", join(config.linkerOptions(), ' '), false);

            out << std::endl;
        }
    }

    //// Compiler options ======================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_ARCHIVER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName.c_str());

            out << "[\"Archiver\" Settings: \"" << configName << "\"]" << std::endl;

            writeConfig(out, "Options", join(config.archiverOptions(), ' '), false);

            out << std::endl;
        }
    }

    //// File custom options ===================================================

    for (const std::string& configName : settings.configs())
    {
        ConfigSettings config = settings.configSettings(configName.c_str());

        std::set<std::string> optionsAddedKeys = keys(config.fileOptionsAdded());
        std::set<std::string> optionsRemovedKeys = keys(config.fileOptionsRemoved());

        std::set<std::string> sources;
        sources.insert(optionsAddedKeys.begin(),   optionsAddedKeys.end());
        sources.insert(optionsRemovedKeys.begin(), optionsRemovedKeys.end());

        for (const std::string& source : sources)
        {
            std::list<std::string> compilerOptions;
            compilerOptions.push_back("\"Compiler\"");

            stringsetmap optionsAdded = config.fileOptionsAdded();
            stringsetmap optionsRemoved = config.fileOptionsRemoved();

            if (optionsAdded.find(source) != optionsAdded.end())
            {
                std::set<std::string> added = optionsAdded.at(source);

                compilerOptions.push_back("+{" + join(added, ' ') + "}");
            }

            if (optionsRemoved.find(source) != optionsRemoved.end())
            {
                std::set<std::string> removed = optionsRemoved.at(source);

                compilerOptions.push_back("-{" + join(removed, ' ') + "}");
            }

            out << "[\"" << source << "\" Settings: \"" << configName << "\"]" << std::endl;
            writeConfig(out, "Options", join(compilerOptions, ' '));

            out << std::endl;
        }
    }

    //// Linker options ========================================================

    for (const std::string& configName : settings.configs())
    {
        ConfigSettings config = settings.configSettings(configName.c_str());
        std::map<std::string, uint> linkOrder = config.fileLinkOrder();

        for (auto it = linkOrder.begin(); it != linkOrder.end(); ++it)
        {
            const std::string& source = it->first;
            std::string order = string_format("%u", it->second);

            out << "[\"" << source << "\" Settings: \"" << configName << "\"]" << std::endl;
            writeConfig(out, "LinkOrder", order, false);

            out << std::endl;
        }
    }

    //// =======================================================================

    return true;
}

void ProjectExportCcs3::writeConfig(std::ostream& out, const char *name, const std::string& value, bool quote)
{
    out << name << "=";

    if (quote)
    {
        out << '\"';
    }

    out << value;

    if (quote)
    {
        out << '\"';
    }

    out << std::endl;
}
