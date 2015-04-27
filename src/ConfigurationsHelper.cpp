#include "StdAfx.h"
#include "ConfigurationsHelper.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"


ConfigurationsHelper::ConfigurationsHelper( VisualStudioProject* project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name ),
      m_configuration_name_Win32( configuration_name + "|Win32" )
{
}


ConfigurationPtr ConfigurationsHelper::get_configuration()
{
    if ( m_project && m_project->m_configurations )
    {
        ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

        for ( size_t i = 0; i < configurations.size(); ++i )
        {
            OptionListHelper configuration_options( &configurations[i]->m_options );

            if ( configuration_options.get_option_value( "Name" ) == m_configuration_name_Win32 )
            {
                return configurations[i];
            }
        }
    }

    return ConfigurationPtr();
}


ToolPtr ConfigurationsHelper::get_tool( const std::string& name )
{
    ConfigurationPtr configuration = get_configuration();

    if ( configuration )
    {
        ToolPtrList tools = configuration->m_tools;

        for ( size_t i = 0; i < tools.size(); ++i )
        {
            OptionListHelper tool_options( &tools[i]->m_options );

            if ( tool_options.get_option_value( "Name" ) == name )
            {
                return tools[i];
            }
        }
    }

    return ToolPtr();
}
