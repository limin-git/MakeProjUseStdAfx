#pragma once
#include "CommonDefs.h"


class VCPreBuildEventToolMaker
{
public:

    VCPreBuildEventToolMaker( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    virtual void make_all();
    void make_CommandLine();

    void remove_vc90_pdb_idb();

public:

    VisualStudioProjectPtr m_project;
    ToolPtr m_tool;
    ConfigurationPtr m_configuration;
    OptionListHelperPtr m_helper;
    std::string m_configuration_name;
};
