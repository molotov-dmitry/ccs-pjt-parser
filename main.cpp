#include "projectreader.h"
#include "export/projectexportccs3.h"

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

    //// Print project =========================================================

    const char* exportPath = nullptr;

    if (argc > 2)
    {
        exportPath = argv[2];
    }

    ProjectExportCcs3 writer;

    if (not writer.write(reader.projectSettings(), exportPath))
    {
        std::cerr << writer.lastError() << std::endl;
        return 3;
    }

    //// =======================================================================

    return 0;
}
