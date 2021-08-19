#include "projectexportmakefile.h"

#include <string.h>

#include "utils.h"

ProjectExportMakefile::ProjectExportMakefile() : mTabWidth(4)
{

}

void ProjectExportMakefile::setTarget(std::string target)
{
    target = basename(target);

    stringlist namelist = split(target, '.');
    if (namelist.size() > 1)
    {
        namelist.pop_back();
        target = join(namelist, '.');
    }

    mTarget = target;
}

void ProjectExportMakefile::setTabWidth(size_t tabWidth)
{
    mTabWidth = tabWidth;
}

std::string getObjectName(const std::string& source)
{
    stringlist pathlist = split(source, '/');

    if (pathlist.size() > 0)
    {
        std::string name = pathlist.back();
        stringlist namelist = split(name, '.');

        if (namelist.size() > 1)
        {
            namelist.pop_back();
            namelist.push_back("obj");
        }

        return join(namelist, '.');
    }

    return source;
}

stringlist getObjects(const stringset& sources)
{
    stringlist objects;

    for (const std::string& source : sources)
    {
        objects.push_back(getObjectName(source));
    }

    return objects;
}

std::string fixVariables(std::string s)
{
    int type = 0;

    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == '%')
        {
            if (type == 0)
            {
                s[i] = '$';
                s.insert(i + 1, "(");
            }
            else
            {
                s[i] = ')';
            }

            type ^= 1;
        }
        else if (s[i] == '\\')
        {
            s[i] = '/';
        }
    }

    return s;
}

void addVariables(stringset& variables, const stringlist& options)
{
    for (const std::string& option : options)
    {
        std::string variable;
        bool inVariable = false;

        for (char c : option)
        {
            if (c != '%')
            {
                if (inVariable)
                {
                    variable.append(&c, 1);
                }
            }
            else
            {
                if (inVariable)
                {
                    variables.insert(variable);
                }
                else
                {
                    variable.clear();
                }

                inVariable = !inVariable;
            }
        }
    }
}

void removeOption(stringlist& optionsList, const std::string& option, bool flag = true)
{
    stringlist result;

    bool removeNext = false;

    for (const std::string& o : optionsList)
    {
        if (not starts_with(o, option) && not removeNext)
        {
            result.push_back(o);
        }

        removeNext = false;

        if (o == option && not flag)
        {
            removeNext = true;
        }
    }

    optionsList = result;
}

bool ProjectExportMakefile::writeData(const ProjectSettings& settings, std::ostream& out)
{
    writeConfig(out, "TARGET", mTarget);
    writeConfig(out, "MAKEFILE", basename(getPath()));
    writeConfig(out, "Proj_dir", "$(CURDIR)");
    out << std::endl;

    //// Tools =================================================================

    uint32_t tools = settings.toolFlags();

    if (tools & ProjectSettings::TOOL_COMPILER)
    {
        writeConfig(out, "CC", "cl6x");
    }
    if (tools & ProjectSettings::TOOL_LINKER)
    {
        writeConfig(out, "LD", "cl6x -z");
    }
    if (tools & ProjectSettings::TOOL_ARCHIVER)
    {
        writeConfig(out, "AR", "ar6x");
    }

    out << std::endl;

    //// Sources paths and objects names =======================================

    stringset sources = settings.sources();
    stringlist objects = getObjects(sources);

    std::map<std::string, std::string> objectSources;

    for (const std::string& source : sources)
    {
        std::string object = getObjectName(source);
        objectSources[object] = source;
    }

    writeConfig(out, "SOURCES", fixVariables(join(sources, ' ')));
    writeConfig(out, "OBJECTS", join(objects, ' '));
    out << std::endl;

    //// Objects directories ===================================================

    for (const std::string& configName : settings.configs())
    {
        writeConfig(out, "OBJDIR_", to_upper(configName), configName);
    }

    out << std::endl;

    //// Objects ===============================================================

    for (const std::string& configName : settings.configs())
    {
        std::string objectPaths = "$(addprefix " + configName + "/,$(OBJECTS))";

        writeConfig(out, "OBJECTS_", to_upper(configName), objectPaths);
    }

    out << std::endl;

    //// Phony targets =========================================================

    stringlist phonyTargets;
    phonyTargets.push_back("all");
    phonyTargets.push_back("clean");
    phonyTargets.push_back("check");

    for (const std::string& configName : settings.configs())
    {
        phonyTargets.push_back(to_lower(configName));

        if (tools & ProjectSettings::TOOL_COMPILER)
        {
            phonyTargets.push_back("obj_" + to_lower(configName));
        }

        phonyTargets.push_back("pre_" + to_lower(configName));
        phonyTargets.push_back("post_" + to_lower(configName));
        phonyTargets.push_back("check_" + to_lower(configName));
    }

    out << ".PHONY: " << join(phonyTargets, ' ') << std::endl;
    out << std::endl;

    //// Main targets ==========================================================

    std::string configsTargets = join(to_lower(settings.configs()), ' ');

    out << "all: " << configsTargets << std::endl;
    out << std::endl;
    out << "clean:" << std::endl;
    out << "\t" << "rm -rf " << join(settings.configs(), ' ') << std::endl;
    out << std::endl;

    //// Configurations ========================================================

    for (const std::string& configName : settings.configs())
    {
        std::string config_l = to_lower(configName);
        std::string config_u = to_upper(configName);

        const ConfigSettings config = settings.configSettings(configName);

        writeComment(out, 1, configName);

        //// Compiler options --------------------------------------------------

        if (tools & ProjectSettings::TOOL_COMPILER)
        {
            writeComment(out, 3, "Compiler options");

            writeConfig(out, "INCLUDES_", config_u, fixVariables(join(config.includePaths(), ' ')));
            writeConfig(out, "DEFINES_", config_u, join(config.defines(), ' '));
            out << std::endl;

            std::string iflags = "$(addsuffix \",$(addprefix -i\",$(INCLUDES_" + config_u + ")))";
            std::string dflags = "$(addsuffix \",$(addprefix -d\",$(DEFINES_" + config_u + ")))";

            writeConfig(out, "IFLAGS_", config_u, iflags);
            writeConfig(out, "DFLAGS_", config_u, dflags);
            out << std::endl;
        }

        //// Linker options ----------------------------------------------------

        if (tools & ProjectSettings::TOOL_LINKER)
        {
            writeComment(out, 3, "Linker options");

            writeConfig(out, "MEM_", config_u,  fixVariables(join(settings.commands(), ' '))); //TODO: replace with ordered list
            writeConfig(out, "LIBS_", config_u, fixVariables(join(settings.libraries(), ' ')));
            out << std::endl;

            std::string mapPath = config.linkerOption("-m", "./$(OBJDIR_" + config_u + ")/$(TARGET).map");
            writeConfig(out, "MAP_", config_u, fixVariables(mapPath));

            std::string outPath = config.linkerOption("-o", "./$(OBJDIR_" + config_u + ")/$(TARGET).out");
            writeConfig(out, "OUT_", config_u, fixVariables(outPath));
            out << std::endl;

            writeConfig(out, "OUT_", config_u + "_DIR", "$(dir $(OUT_" + config_u + "))");
            out << std::endl;

            stringlist linkerOptions = config.linkerOptions();
            removeOption(linkerOptions, "-m", false);
            removeOption(linkerOptions, "-o", false);
            linkerOptions.push_back("-m");
            linkerOptions.push_back("$(MAP_" + config_u + ")");
            linkerOptions.push_back("-o");
            linkerOptions.push_back("$(OUT_" + config_u + ")");
            writeConfig(out, "LDFLAGS_", config_u, join(linkerOptions, ' '));
            out << std::endl;

            writeComment(out, 5, "Link");

            out << config_l << ": $(OUT_" << config_u << ")" << std::endl;
            out << std::endl;

            out << string_format("$(OUT_%s): $(MEM_%s) $(OBJDIR_%s)/pre_build $(OBJECTS_%s) $(LIBS_%s)",
                                 config_u.c_str(),
                                 config_u.c_str(),
                                 config_u.c_str(),
                                 config_u.c_str(),
                                 config_u.c_str()) << std::endl;

            out << "\t" << "mkdir -p $(OUT_" << config_u << "_DIR)" << std::endl;
            out << "\t" << string_format("$(LD) $(LDFLAGS_%s) $(MEM_%s) $(OBJECTS_%s) $(LIBS_%s)",
                                         config_u.c_str(),
                                         config_u.c_str(),
                                         config_u.c_str(),
                                         config_u.c_str()) << std::endl;

            out << std::endl;

        }

        //// Prebuild ----------------------------------------------------------

        writeComment(out, 2, "Prebuild");

        out << "pre_" << config_l << ": $(OBJDIR_" << config_u << ")/pre_build" << std::endl;
        out << std::endl;

        out << string_format("$(OBJDIR_%s)/pre_build: $(MEM_%s) $(SOURCES) $(LIBS_%s) $(MAKEFILE)",
                             config_u.c_str(),
                             config_u.c_str(),
                             config_u.c_str()) << std::endl;

        out << "\t" << "mkdir -p $(OBJDIR_" << config_u << ")" << std::endl;

        for (const BuildStep& prebuild : config.preBuildSteps()) //TODO: Add always build targets
        {
            out << "\t" << fixVariables(cp1251_to_unicode(prebuild.command())) << std::endl;
        }

        out << "\t" << "touch $@" << std::endl;
        out << std::endl;

        //// Postbuild ---------------------------------------------------------

        writeComment(out, 2, "Postbuild");

        out << "post_" << config_l << ": $(OBJDIR_" << config_u << ")/post_build" << std::endl;
        out << std::endl;

        out << string_format("$(OBJDIR_%s)/post_build: $(OUT_%s) $(MAKEFILE)",
                             config_u.c_str(),
                             config_u.c_str()) << std::endl;

        for (const BuildStep& postbuild : config.postBuildSteps()) //TODO: Add always build targets
        {
            out << "\t" << fixVariables(cp1251_to_unicode(postbuild.command())) << std::endl;
        }

        out << "\t" << "touch $@" << std::endl;
        out << std::endl;

        //// Object files ------------------------------------------------------

        if (tools & ProjectSettings::TOOL_COMPILER)
        {
            writeComment(out, 2, "Object files");

            out << "obj_" << config_l << ": $(OBJECTS_" << config_u << ")" << std::endl;
            out << std::endl;

            out << "$(OBJDIR_" << config_u << "):" << std::endl;
            out << "\t" << "mkdir -p $@" << std::endl;
            out << std::endl;

            for (const std::string& object : objects)
            {
                const std::string& source = objectSources.at(object);
                FileOptions fileOptions = config.fileOptions(source);

                stringlist compilerOptions = config.compilerOptions();

                for (const std::string& optionRemove : fileOptions.optionsRemoved())
                {
                    removeOption(compilerOptions, optionRemove);
                }

                for (const std::string& optionAdd : fileOptions.optionsAdded())
                {
                    compilerOptions.push_back(optionAdd);
                }

                removeOption(compilerOptions, "-fr", false);

                compilerOptions.push_back("-fr $(OBJDIR_" + config_u + ")");

                out << "$(OBJDIR_" << config_u << ")/" << object << ": " << source << " $(MAKEFILE)" << std::endl;
                out << "\t" << /*"cd $(dir " << source << ") && " <<*/ "$(CC) " << join(compilerOptions, ' ') << " $(IFLAGS_" << config_u << ") $(DFLAGS_" << config_u << ") " << source << std::endl;
                out << std::endl;
            }
        }

        //// Checks ------------------------------------------------------------

        stringlist buildSteps;
        for (const BuildStep& step : config.preBuildSteps()) //TODO: Add always build targets
        {
            buildSteps.push_back(step.command());
        }
        for (const BuildStep& step : config.postBuildSteps()) //TODO: Add always build targets
        {
            buildSteps.push_back(step.command());
        }

        std::set<std::string> variables;
        addVariables(variables, config.includePaths());
        addVariables(variables, config.defines());
        addVariables(variables, config.undefines());
        addVariables(variables, config.compilerOptions());
        addVariables(variables, config.linkerOptions());
        addVariables(variables, config.archiverOptions());
        addVariables(variables, buildSteps);

        writeComment(out, 2, "Checks");

        out << "check_" << config_l << ": check" << std::endl;

        for (const std::string& variable : variables)
        {
            out << "\t" << "@echo 'check " << variable << " variable' && test -n \"$(" << variable << ")\" > /dev/null" << std::endl;
        }

        out << std::endl;
    }

    //// Checks ================================================================

    writeComment(out, 1, "Checks");

    out << "check:" << std::endl;
    out << "\t" << "@echo 'check C6X_C_DIR' && test -n '$(C6X_C_DIR)'" << std::endl;

    if (tools & ProjectSettings::TOOL_COMPILER)
    {
        out << "\t" << "@echo 'check CC executable' && which $(firstword $(CC)) > /dev/null" << std::endl;
    }

    if (tools & ProjectSettings::TOOL_LINKER)
    {
        out << "\t" << "@echo 'check LD executable' && which $(firstword $(LD)) > /dev/null" << std::endl;
    }

    if (tools & ProjectSettings::TOOL_ARCHIVER)
    {
        out << "\t" << "@echo 'check AR executable' && which $(firstword $(AR)) > /dev/null" << std::endl;
    }

    out << std::endl;

    return true;
}

void ProjectExportMakefile::writeConfig(std::ostream& out, const char* name, const std::string& value, bool constant)
{
    const size_t NAME_LEN_MAX = 20;

    size_t name_len = strlen(name);
    size_t spaces   = 1;

    if (name_len < NAME_LEN_MAX)
    {
        spaces = NAME_LEN_MAX - name_len;
    }

    out << name;

    for (size_t i = 0; i < spaces; ++i)
    {
        out << " ";
    }

    if (constant)
    {
        out << ":= ";
    }
    else
    {
        out << "=  ";
    }

    out << value;

    out << std::endl;
}

void ProjectExportMakefile::writeConfig(std::ostream& out, const char* name, const std::string& nameSuffix, const std::string& value, bool constant)
{
    std::string nameFull = std::string(name) + nameSuffix;
    writeConfig(out, nameFull.c_str(), value, constant);
}

void justifiedPrintComment(std::ostream& out, size_t prefixLen, char suffix, const std::string& name = std::string())
{
    const size_t WIDTH = 80;

    std::string print;
    print.reserve(80);

    for (size_t i = 0; i < prefixLen; ++i)
    {
        print.push_back('#');
    }

    print.push_back(' ');

    print.append(name);

    if (name.size() > 0)
    {
        print.push_back(' ');
        ++prefixLen;
    }

    for (size_t len = prefixLen + 1 + name.size(); len < WIDTH; ++len)
    {
        print.push_back(suffix);
    }

    out << print << std::endl;
}

void ProjectExportMakefile::writeComment(std::ostream& out, size_t level, const std::string& name, bool emptyLine)
{
    if (level == 1)
    {
        justifiedPrintComment(out, 3, '=');
        justifiedPrintComment(out, 3, '=', name);
        justifiedPrintComment(out, 3, '=');
    }
    else if (level == 2)
    {
        justifiedPrintComment(out, 3, '=');
        justifiedPrintComment(out, 3, '-', name);
        justifiedPrintComment(out, 3, '=');
    }
    else if (level == 3)
    {
        justifiedPrintComment(out, 3, '-');
        justifiedPrintComment(out, 3, '=', name);
        justifiedPrintComment(out, 3, '-');
    }
    else if (level == 4)
    {
        justifiedPrintComment(out, 3, '-');
        justifiedPrintComment(out, 3, '-', name);
        justifiedPrintComment(out, 3, '-');
    }
    else if (level == 5)
    {
        justifiedPrintComment(out, 3, '=', name);
    }
    else
    {
        justifiedPrintComment(out, 3, '-', name);
    }

    if (emptyLine)
    {
        out << std::endl;
    }
}
