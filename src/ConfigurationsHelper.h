#pragma once
#include "CommonDefs.h"


class ConfigurationsHelper
{
public:

    ConfigurationsHelper( VisualStudioProject* project, const std::string& configuration_name = "Debug" );

    ConfigurationPtr get_configuration();
    ToolPtr get_tool( const std::string& name );

public:

    VisualStudioProject* m_project;
    std::string m_configuration_name;
    std::string m_configuration_name_Win32;
};
