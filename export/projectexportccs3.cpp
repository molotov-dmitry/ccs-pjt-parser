#include "projectexportccs3.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "../utils.h"

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

        ConfigSettings config = settings.configSettings(configName);

        for (const BuildStep& step : config.preBuildSteps())
        {
            writeConfig(out, "InitialBuildCmd", step.toString(), false);
        }

        for (const BuildStep& step : config.postBuildSteps())
        {
            writeConfig(out, "FinalBuildCmd", step.toString(), false);
        }

        out << std::endl;
    }

    //// Compiler options ======================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_COMPILER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName);

            out << "[\"Compiler\" Settings: \"" << configName << "\"]" << std::endl;

            std::list<std::string> options = config.otherCompilerOptions();

            for (const std::string& include : config.includePaths())
            {
                options.push_back(to_option("-i", include));
            }

            for (const std::string& define : config.defines())
            {
                options.push_back(to_option("-d", define));
            }

            for (const std::string& undefine : config.undefines())
            {
                options.push_back("-u\"" + undefine + "\"");
            }

            writeConfig(out, "Options", join(options, ' '), false);

            out << std::endl;
        }
    }

    //// Linker options ========================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_LINKER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName);

            out << "[\"Linker\" Settings: \"" << configName << "\"]" << std::endl;

            std::list<std::string> options = config.otherLinkerOptions();

            if (not config.mapFile().empty())
            {
                options.push_back(to_option("-m", config.mapFile()));
            }

            if (not config.outputFile().empty())
            {
                options.push_back(to_option("-o", config.outputFile()));
            }

            for (const std::string& libraryPath : config.libraryPaths())
            {
                options.push_back(to_option("-i", libraryPath));
            }

            for (const std::string& lib : config.libraries())
            {
                options.push_back(to_option("-l", lib));
            }

            writeConfig(out, "Options", join(options, ' '), false);

            out << std::endl;
        }
    }

    //// Archiver options ======================================================

    if (settings.toolFlags() & ProjectSettings::TOOL_ARCHIVER)
    {
        for (const std::string& configName : settings.configs())
        {
            ConfigSettings config = settings.configSettings(configName);

            out << "[\"Archiver\" Settings: \"" << configName << "\"]" << std::endl;

            writeConfig(out, "Options", join(config.otherArchiverOptions(), ' '), false);

            out << std::endl;
        }
    }

    //// File custom options ===================================================

    for (const std::string& configName : settings.configs())
    {
        for (const std::string& source : settings.files())
        {
            FileOptions option = settings.configSettings(configName).fileOptions(source);
            if (not option.isDefault())
            {
                out << "[\"" << source << "\" Settings: \"" << configName << "\"]" << std::endl;

                //// Compiler options ------------------------------------------

                stringset added   = option.optionsAdded();
                stringset removed = option.optionsRemoved();

                if (not added.empty() || not removed.empty())
                {
                    std::list<std::string> compilerOptions;
                    compilerOptions.push_back("\"Compiler\"");

                    if (not added.empty())
                    {
                        compilerOptions.push_back("+{" + join(added, ' ') + "}");
                    }

                    if (not removed.empty())
                    {
                        compilerOptions.push_back("-{" + join(removed, ' ') + "}");
                    }

                    writeConfig(out, "Options", join(compilerOptions, ' '), false);
                }

                //// Link Order ------------------------------------------------

                if (option.linkOrder() >= 0)
                {
                    std::string order = string_format("%d", option.linkOrder());
                    writeConfig(out, "LinkOrder", order, false);
                }

                //// Exclude from build ----------------------------------------

                if (option.isExcludedFromBuild())
                {
                    writeConfig(out, "ExcludeFromBuild", "true", false);
                }

                //// Run condition ---------------------------------------------

                if (option.buildCondition() != BuildStep::IF_ANY_FILE_BUILDS)
                {
                    std::string buildConditionStr = BuildStep::buildConditionString(option.buildCondition(), true);
                    writeConfig(out, "Run", buildConditionStr, false);
                }

                //// Build commands --------------------------------------------

                for (const BuildStep& step : option.preBuildSteps().get())
                {
                    writeConfig(out, "PreBuildCmd", step.toString(), false);
                }

                for (const BuildStep& step : option.postBuildSteps().get())
                {
                    writeConfig(out, "PostBuildCmd", step.toString(), false);
                }

                //// -----------------------------------------------------------

                out << std::endl;
            }
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
