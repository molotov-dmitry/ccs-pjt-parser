#include "projectreader.h"
#include "export/projectexportccs3.h"
#include "export/projectexportmakefile.h"
#include "export/projectexportqtmakefile.h"

#include <iostream>
#include <strings.h>

#include "utils.h"

enum Args
{
    ARG_EXEC,
    ARG_IN_FILE,
    ARG_OUT_FORMAT,
    ARG_OUT_FILE
};

enum OutputFormats
{
    OF_PJT,
    OF_MAKEFILE,
    OF_QT_MAKE_SOURCES,
    OF_QT_MAKE_DEFINES,
    OF_QT_MAKE_INCLUDES,

    OF_COUNT
};

static const char* const FORMAT_NAMES[OF_COUNT] =
{
    "pjt",
    "make",
    "qt_make_sources",
    "qt_make_defines",
    "qt_make_includes"
};

void usage(const char* exec)
{
    std::cerr << "Usage: " << exec
              << " input [format1 output1] [format2 output2]..."
              << std::endl;
}

int main(int argc, char* argv[])
{
    //// Check argument count ==================================================

    if (argc <= ARG_IN_FILE)
    {
        usage(argv[0]);
        std::cerr << "Missing project path argument" << std::endl;
        return 1;
    }

    //// Read project file =====================================================

    ProjectReader reader(argv[1]);

    if (not reader.read())
    {
        std::cerr << reader.lastError() << std::endl;
        return 2;
    }

    ProjectSettings settings = reader.projectSettings();

    //// Write output ==========================================================

    int currIndex = 0;

    while (true)
    {
        OutputFormats format = OF_COUNT;

        //// Get output file options -------------------------------------------

        if (argc <= ARG_OUT_FORMAT + currIndex)
        {
            break;
        }

        for (size_t i = 0; i < OF_COUNT; ++i)
        {
            if (strcasecmp(argv[ARG_OUT_FORMAT + currIndex], FORMAT_NAMES[i]) == 0)
            {
                format = (OutputFormats)i;
                break;
            }
        }

        if (format == OF_COUNT)
        {
            usage(argv[0]);
            std::cerr << "Wrong output format: "
                      << argv[ARG_OUT_FORMAT + currIndex]
                      << std::endl;

            stringlist formats;

            for (size_t i = 0; i < OF_COUNT; ++i)
            {
                formats.push_back(FORMAT_NAMES[i]);
            }

            std::cerr << "Available formats: "
                      << join(formats, ' ')
                      << std::endl;
        }

        if (argc <= ARG_OUT_FILE + currIndex)
        {
            usage(argv[0]);
            std::cerr << "Missing output path argument" << std::endl;
            return 1;
        }

        //// Print output ------------------------------------------------------

        AbstractProjectExport* writer = nullptr;

        switch (format)
        {
        case OF_PJT:
        {
            writer = new ProjectExportCcs3;
            break;
        }

        case OF_MAKEFILE:
        {
            ProjectExportMakefile* writerMakefile = new ProjectExportMakefile;
            writerMakefile->setTarget(argv[ARG_OUT_FILE + currIndex]);
            writer = writerMakefile;
            break;
        }

        case OF_QT_MAKE_SOURCES:
        {
            writer = new ProjectExportQtMakefileSources();
            break;
        }

        case OF_QT_MAKE_DEFINES:
        {
            writer = new ProjectExportQtMakefileDefines(argv[ARG_OUT_FILE + currIndex]);
            ++currIndex;
            break;
        }

        case OF_QT_MAKE_INCLUDES:
        {
            writer = new ProjectExportQtMakefileIncludes(argv[ARG_OUT_FILE + currIndex]);
            ++currIndex;
            break;
        }

        case OF_COUNT:
        {
            break;
        }
        }

        if (writer == nullptr)
        {
            std::cerr << "Internal error" << std::endl;
            return 3;
        }

        if (not writer->write(settings, argv[ARG_OUT_FILE + currIndex]))
        {
            std::cerr << writer->lastError() << std::endl;
            return 3;
        }

        //// -------------------------------------------------------------------

        currIndex += 2;
    }

    //// =======================================================================

    return 0;
}
