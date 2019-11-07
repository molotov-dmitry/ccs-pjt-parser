#include "projectparser.h"

#include <string.h>
#include <memory>

#include "utils.h"

ProjectParser::ProjectParser() : mSectionType(SectionType::NONE)
{

}

bool ProjectParser::parseLine(const char* line_c)
{
    std::string line = std::string(line_c);

    if (isSection(line))
    {
        return parseSection(line.substr(1, line.length() - 2));
    }
    else
    {
        return parseData(line);
    };
}

void ProjectParser::clear()
{
    mProjectSettings.clear();
}

std::string ProjectParser::lastError() const
{
    return mLastError;
}

ProjectSettings ProjectParser::projectSettings() const
{
    return mProjectSettings;
}

//// ===========================================================================
//// Sections ==================================================================
//// ===========================================================================

bool ProjectParser::isSection(const std::string& line) const
{
    if (line.length() < 2)
    {
        return false;
    }

    return (starts_with(line, "[") && ends_with(line, "]"));
}

bool ProjectParser::isConfigSettingsSection(const std::string& line, std::string& config) const
{
    std::unique_ptr<char[]> config_buf(new char[line.size() + 1]);

    int chars = 0;

    if (sscanf(line.c_str(), "%s Settings%n", config_buf.get(), &chars) != 1)
    {
        return false;
    }

    if (line.c_str()[chars] != '\0')
    {
        return false;
    }

    config = std::string(config_buf.get());

    if (not(remove_quotes(config)))
    {
        return false;
    }

    return true;
}

bool ProjectParser::getFileSettingsSection(const std::string& line, std::string& config, std::string& file, const stringset& file_set) const
{
    std::unique_ptr<char[]> config_buf(new char[line.size() + 1]);
    std::unique_ptr<char[]> file_buf(new char[line.size() + 1]);

    int chars = 0;

    if (sscanf(line.c_str(), "%s Settings: %s%n", file_buf.get(), config_buf.get(), &chars) != 2)
    {
        return false;
    }

    if (line.c_str()[chars] != '\0')
    {
        return false;
    }

    config = std::string(config_buf.get());
    file   = std::string(file_buf.get());

    if (not(remove_quotes(config)))
    {
        return false;
    }

    if (not remove_quotes(file))
    {
        return false;
    }

    if (file_set.find(file) == file_set.end())
    {
        return false;
    }

    return true;
}

bool ProjectParser::isToolSettingsSection(const std::string& line, std::string& config, std::string& tool) const
{
    return getFileSettingsSection(line, config,
                                  tool, mProjectSettings.c_tools());
}

bool ProjectParser::isSourceSettingsSection(const std::string& line, std::string& config, std::string& source) const
{
    return getFileSettingsSection(line, config,
                                  source, mProjectSettings.c_sources());
}

bool ProjectParser::isLibrarySettingsSection(const std::string& line, std::string& config, std::string& library) const
{
    return getFileSettingsSection(line, config,
                                  library, mProjectSettings.c_libraries());
}

bool ProjectParser::isCommandSettingsSection(const std::string& line, std::string& config, std::string& command) const
{
    return getFileSettingsSection(line, config,
                                  command, mProjectSettings.c_commands());
}

bool ProjectParser::parseSection(const std::string& line)
{
    if (strcasecmp(line.c_str(), "Project Settings") == 0)
    {
        mSectionType = SectionType::PROJECT_SETTINGS;
    }
    else if (strcasecmp(line.c_str(), "Source Files") == 0)
    {
        mSectionType = SectionType::SOURCE_FILES;
    }
    else if (isConfigSettingsSection(line, mCurrentConfig))
    {
        mSectionType = SectionType::CONFIG_SETTINGS;
    }
    else if (isToolSettingsSection(line, mCurrentConfig, mCurrentTool))
    {
        mSectionType = SectionType::TOOL_SETTINGS;
    }
    else if (isSourceSettingsSection(line, mCurrentConfig, mCurrentFile))
    {
        mSectionType = SectionType::SOURCE_SETTINGS;
    }
    else if (isLibrarySettingsSection(line, mCurrentConfig, mCurrentFile))
    {
        mSectionType = SectionType::LIBRARY_SETTINGS;
    }
    else if (isCommandSettingsSection(line, mCurrentConfig, mCurrentFile))
    {
        mSectionType = SectionType::COMMAND_SETTINGS;
    }
    else
    {
        mSectionType = SectionType::NONE;
    };

    return true;
}

//// ===========================================================================
//// Data ======================================================================
//// ===========================================================================

bool ProjectParser::parseData(const std::string& line)
{
    std::string key;
    std::string val;

    if (not(split_config_line(line, key, val)))
    {
        mLastError = string_format("Unknown config line: '%s'", line.c_str());
        return false;
    }

    switch (mSectionType)
    {
    case SectionType::PROJECT_SETTINGS:
        return parseProjectSettings(key, val);

    case SectionType::SOURCE_FILES:
        return parseSourceFile(key, val);

    case SectionType::CONFIG_SETTINGS:
        return parseConfigSettings(key, val);

    case SectionType::TOOL_SETTINGS:
        return parseToolSettings(key, val);

    case SectionType::SOURCE_SETTINGS:
    case SectionType::LIBRARY_SETTINGS:
    case SectionType::COMMAND_SETTINGS:
        return parseSourceSettings(key, val);

    case SectionType::NONE:
        return true;
    }

    return false;
}

//// ===========================================================================
//// Project Settings ----------------------------------------------------------
//// ===========================================================================

bool ProjectParser::parseProjectSettings(const std::string& key, const std::string& value)
{
    //// Project type ==========================================================

    if (strcasecmp(key.c_str(), "ProjectType") == 0)
    {
        if (strcasecmp(value.c_str(), "Executable") == 0)
        {
            mProjectSettings.setProjectType(ProjectSettings::Type::EXECUTABLE);
        }
        else if (strcasecmp(value.c_str(), "Library") == 0)
        {
            mProjectSettings.setProjectType(ProjectSettings::Type::LIBRARY);
        }
        else
        {
            mLastError = string_format("Unknown project type '%s'",
                                       value.c_str());

            return false;
        }
    }

    //// Tools =================================================================

    else if (strcasecmp(key.c_str(), "Tool") == 0)
    {
        mProjectSettings.addTool(value.c_str());
    }

    //// Configs ===============================================================

    else if (strcasecmp(key.c_str(), "Config") == 0)
    {
        mProjectSettings.addConfig(value.c_str());
    }

    //// CPU family ============================================================

    else if (strcasecmp(key.c_str(), "CPUFamily") == 0)
    {
        mProjectSettings.setCpuFamily(value.c_str());
    }

    //// Project path ==========================================================

    else if (strcasecmp(key.c_str(), "ProjectDir") == 0)
    {
        mProjectSettings.setProjectDir(value.c_str());
    }

    //// Unknown ===============================================================

    else
    {
        fprintf(stderr, "Warning: unknown Project settings configuration key '%s'\n", key.c_str());
    }

    //// =======================================================================

    return true;
}

//// ===========================================================================
//// Source file ---------------------------------------------------------------
//// ===========================================================================

bool ProjectParser::parseSourceFile(const std::string& key, const std::string& value)
{
    //// Source ================================================================

    if (strcasecmp(key.c_str(), "Source") == 0)
    {
        mProjectSettings.addSource(value.c_str());
    }

    //// Unknown ===============================================================

    else
    {
        mLastError = string_format("Unknown source file list key '%s'", key.c_str());

        return false;
    };

    //// =======================================================================

    return true;
}

//// ===========================================================================
//// Config settings -----------------------------------------------------------
//// ===========================================================================

bool ProjectParser::parseConfigSettings(const std::string& key, const std::string& value)
{
    //// Pre build step ========================================================

    if (strcasecmp(key.c_str(), "InitialBuildCmd") == 0)
    {
        mProjectSettings.config(mCurrentConfig).preBuildStepsRef().add(value);
    }

    //// Post build step =======================================================

    else if (strcasecmp(key.c_str(), "FinalBuildCmd") == 0)
    {
        mProjectSettings.config(mCurrentConfig).postBuildStepsRef().add(value);
    }

    //// Unknown ===============================================================

    else
    {
        mLastError = string_format("Unknown configuration key '%s'", key.c_str());

        return false;
    };

    //// =======================================================================

    return true;
}

bool ProjectParser::parseToolSettings(const std::string& key, const std::string& value)
{
    //// Options ===============================================================

    if (strcasecmp(key.c_str(), "Options") == 0)
    {
        stringlist options = split(value, ' ');

        for (const std::string& option : options)
        {
            if (strcasecmp(mCurrentTool.c_str(), "Compiler") == 0)
            {
                mProjectSettings.config(mCurrentConfig).addCompilerOption(option.c_str());
            }
            else if (strcasecmp(mCurrentTool.c_str(), "Linker") == 0)
            {
                mProjectSettings.config(mCurrentConfig).addLinkerOption(option.c_str());
            }
            else if (strcasecmp(mCurrentTool.c_str(), "Archiver") == 0)
            {
                mProjectSettings.config(mCurrentConfig).addArchiverOption(option.c_str());
            }
            else
            {
                mLastError = string_format("Unknown tool: '%s'", mCurrentTool.c_str());
                return false;
            }
        }
    }

    //// Unknown ===============================================================

    else
    {
        mLastError = string_format("Unknown tool configuration key '%s'", key.c_str());

        return false;
    };

    //// =======================================================================

    return true;
}

bool ProjectParser::parseSourceSettings(const std::string& key, const std::string& value)
{
    //// Options ===============================================================

    if (strcasecmp(key.c_str(), "Options") == 0)
    {
        //// Compiler ----------------------------------------------------------

        if (starts_with(value, "\"Compiler\" "))
        {
            std::string opt_add;

            if (between(value, "+{", "}", opt_add))
            {
                for (const std::string& option : split(opt_add, ' '))
                {
                    mProjectSettings.config(mCurrentConfig).file(mCurrentFile).addOptionAdded(option);
                }
            }

            std::string opt_del;

            if (between(value, "-{", "}", opt_del))
            {
                for (const std::string& option : split(opt_del, ' '))
                {
                    mProjectSettings.config(mCurrentConfig).file(mCurrentFile).addOptionRemoved(option);
                }
            }
        }

        //// Unknown -----------------------------------------------------------

        else
        {
            mLastError = string_format("Unknown option value '%s' for source file '%s' options in configuration '%s'",
                                       value.c_str(),
                                       mCurrentFile.c_str(),
                                       mCurrentConfig.c_str());

            return false;
        }
    }

    //// Link order ============================================================

    else if (strcasecmp(key.c_str(), "LinkOrder") == 0)
    {
        uint order = 0;
        int bytes = 0;

        if (sscanf(value.c_str(), "%u%n", &order, &bytes) != 1)
        {
            mLastError = string_format("Wrong link order value '%s' for file '%s' in configuration '%s'",
                                       value.c_str(),
                                       mCurrentFile.c_str(),
                                       mCurrentConfig.c_str());
            return false;
        }

        if (value.c_str()[bytes] != '\0')
        {
            mLastError = string_format("Wrong link order value '%s' for file '%s' in configuration '%s'",
                                       value.c_str(),
                                       mCurrentFile.c_str(),
                                       mCurrentConfig.c_str());
            return false;
        }

        mProjectSettings.config(mCurrentConfig).file(mCurrentFile).setLinkOrder(order);
    }

    //// Run condition =========================================================

    else if (strcasecmp(key.c_str(), "Run") == 0)
    {
        bool found = false;

        for (int i = 0; i < BuildStep::BUILD_CONDITION_COUNT; ++i)
        {
            if (BuildStep::buildConditionString(i, true) == value)
            {
                found = true;
                mProjectSettings.config(mCurrentConfig).file(mCurrentFile).setBuildCondition(i);
                break;
            }
        }

        if (not found)
        {
            mLastError = string_format("Wrong run condition value '%s' for file '%s' in configuration '%s'",
                                       value.c_str(),
                                       mCurrentFile.c_str(),
                                       mCurrentConfig.c_str());
            return false;
        }
    }

    //// Pre build step ========================================================

    else if (strcasecmp(key.c_str(), "PreBuildCmd") == 0)
    {
        mProjectSettings.config(mCurrentConfig).file(mCurrentFile).preBuildSteps().add(BuildStep::fromString(value));
    }

    //// Post build step =======================================================

    else if (strcasecmp(key.c_str(), "PostBuildCmd") == 0)
    {
        mProjectSettings.config(mCurrentConfig).file(mCurrentFile).postBuildSteps().add(BuildStep::fromString(value));
    }

    //// Exclude from build ====================================================

    else if (strcasecmp(key.c_str(), "ExcludeFromBuild") == 0)
    {
        if (strcasecmp(value.c_str(), "true") == 0)
        {
            mProjectSettings.config(mCurrentConfig).file(mCurrentFile).setExcludeFromBuild(true);
        }
        else if (strcasecmp(value.c_str(), "false") == 0)
        {
            mProjectSettings.config(mCurrentConfig).file(mCurrentFile).setExcludeFromBuild(false);
        }
        else
        {
            mLastError = string_format("Wrong exclude from build value '%s' for file '%s' in configuration '%s'",
                                       value.c_str(),
                                       mCurrentFile.c_str(),
                                       mCurrentConfig.c_str());
            return false;
        }


    }

    //// Unknown ===============================================================

    else
    {
        mLastError = string_format("Unknown option key '%s' for file '%s' in configuration '%s'",
                                   key.c_str(),
                                   mCurrentFile.c_str(),
                                   mCurrentConfig.c_str());

        return false;
    };

    //// =======================================================================

    return true;
}
