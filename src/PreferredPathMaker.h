#pragma once
#include "CommonDefs.h"


class PreferredPathMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );
    void make_preferred_path();
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
