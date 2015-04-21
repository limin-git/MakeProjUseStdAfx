#pragma once
#include "CommonDefs.h"


class VCPreBuildEventToolMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug"  );

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );
    void make_CommandLine();
    void remove_vc90_pdb_idb();

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    ConfigurationPtr m_configuration;
    ToolPtr m_tool;
    OptionListHelperPtr m_tool_options;
};
