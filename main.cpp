#include "projectreader.h"
#include "export/projectexportccs3.h"
#include "export/projectexportmakefile.h"

#include <iostream>

int main(int argc, char* argv[])
{
    //// Check argument count ==================================================

    if (argc < 2)
    {
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

    //// Print project =========================================================

    const char* exportPathPjt = nullptr;

    if (argc > 2)
    {
        exportPathPjt = argv[2];
    }

    ProjectExportCcs3 writer;

    if (not writer.write(settings, exportPathPjt))
    {
        std::cerr << writer.lastError() << std::endl;
        return 3;
    }

    //// Print makefile ========================================================

    const char* exportPathMakefile = nullptr;

    if (argc > 3)
    {
        exportPathMakefile = argv[3];
    }

    ProjectExportMakefile writerMakefile;
    writerMakefile.setTarget(argv[1]);
    writerMakefile.setTabWidth(8);

    if (not writerMakefile.write(settings, exportPathMakefile))
    {
        std::cerr << writerMakefile.lastError() << std::endl;
        return 3;
    }

    //// =======================================================================

    return 0;
}
