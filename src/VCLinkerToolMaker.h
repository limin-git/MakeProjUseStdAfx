#pragma once
#include "CommonDefs.h"
#include "ToolMaker.h"


class VCLinkerToolMaker : ToolMaker
{
public:

    VCLinkerToolMaker()
        : ToolMaker( "VCLinkerTool" )
    {
    }

    virtual void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void make_AdditionalLibraryDirectories();
};
