#pragma once
#include "Tool.h"


class VCPreBuildEventTool : public Tool
{
public:

    VCPreBuildEventTool( Vcproj* project, const std::string& str, size_t pos )
        : Tool( project, str, pos )
    {
    }

    void make_CommandLine();
};
