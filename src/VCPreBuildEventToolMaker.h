#pragma once
#include "CommonDefs.h"


class VCPreBuildEventToolMaker
{
public:

    VCPreBuildEventToolMaker( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    virtual void make_all();
    void make_CommandLine();

public:

    VisualStudioProjectPtr m_project;
    ToolPtr m_tool;
    OptionListHelperPtr m_helper;
    std::string m_configuration_name;
};
