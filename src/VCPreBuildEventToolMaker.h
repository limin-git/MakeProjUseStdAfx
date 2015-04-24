#pragma once
#include "CommonDefs.h"
#include "ToolMaker.h"


class VCPreBuildEventToolMaker : ToolMaker
{
public:

    VCPreBuildEventToolMaker()
        : ToolMaker( "VCPreBuildEventTool" )
    {
    }

    virtual void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug"  );

private:

    void make_CommandLine();
    void remove_vc90_xdb();
};
