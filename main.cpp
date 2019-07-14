#include "projectreader.h"

#include <iostream>

int main(int argc, char* argv[])
{

    //// Check argument count ==================================================

    if (argc < 2)
    {
        fprintf(stderr, "Missing project path argument");
        return 1;
    }

    //// Read project file =====================================================

    ProjectReader reader(argv[1]);

    if (not reader.read())
    {
        std::cerr << reader.lastError() << std::endl;
        return 2;
    }

    //// =======================================================================

    return 0;
}
