#include "abstractprojectexport.h"

#include <string.h>
#include <iostream>

#include "../utils.h"

AbstractProjectExport::AbstractProjectExport()
{
}

AbstractProjectExport::~AbstractProjectExport()
{

}

bool AbstractProjectExport::write(const ProjectSettings &settings, const char *path)
{
    std::ostream* out = &std::cout;

    if (path != nullptr)
    {
        mPath = path;
    }

    mLastError.clear();

    //// Open project file =====================================================

    std::ofstream projectFile;

    if (not mPath.empty())
    {
        projectFile.open(mPath.c_str());
        if (not projectFile.is_open())
        {
            mLastError = string_format("Failed to open project '%s': '%s'", mPath.c_str(), strerror(errno));
            return false;
        }

        out = &projectFile;
    }

    //// Write project =========================================================

    bool res = writeData(settings, *out);

    //// Close project file ====================================================

    projectFile.close();

    //// =======================================================================

    return res;
}

std::string AbstractProjectExport::lastError() const
{
    return mLastError;
}
