#pragma once
#include "CommonDefs.h"


class PreferredPathMaker
{
public:

    PreferredPathMaker( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    void make_all();
    void make_preferred_path();

private:

    void make_preferred_path_4_option( OptionListHelperPtr options, const std::string& option_name );

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;

    ConfigurationPtr m_configuration;
    OptionListHelperPtr m_configuration_options;

    ToolPtr m_VCCLCompilerTool;
    OptionListHelperPtr m_VCCLCompilerTool_options;

    ToolPtr m_VCLibrarianTool;
    OptionListHelperPtr m_VCLibrarianTool_options;

    ToolPtr m_VCLinkerTool;
    OptionListHelperPtr m_VCLinkerTool_options;

    ToolPtr m_VCBscMakeTool;
    OptionListHelperPtr m_VCBscMakeTool_options;
};
