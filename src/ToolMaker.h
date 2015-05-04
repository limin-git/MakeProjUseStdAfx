#pragma once
#include "CommonDefs.h"


class ToolMaker
{
public:

    ToolMaker( const std::string& tool_name );
    virtual ~ToolMaker() {}

    virtual void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" ) = 0;

protected:

    virtual void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    std::string m_configuration_name_Win32;
    ConfigurationPtr m_configuration;
    ToolPtr m_tool;
    OptionListHelperPtr m_tool_options;
    std::string m_tool_name;
};
