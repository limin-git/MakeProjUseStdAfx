#include "StdAfx.h"
#include "ToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"


ToolMaker::ToolMaker( const std::string& tool_name )
    : m_tool_name( tool_name )
{
}


void ToolMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;
    m_configuration_name_Win32 = configuration_name + "|Win32";
    m_configuration.reset();
    m_tool.reset();
    m_tool_options.reset();

    if ( false == m_project->m_files_helper->is_exist( ".cpp" ) )
    {
        return;
    }

    ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

    for ( size_t i = 0; i < configurations.size(); ++i )
    {
        ConfigurationPtr configuration = configurations[i];
        OptionListHelper configuration_options( &configuration->m_options );

        if ( configuration_options.get_option_value( "Name" ) == m_configuration_name_Win32 )
        {
            ToolPtrList& tools = configuration->m_tools;

            for ( size_t i = 0; i < tools.size(); ++i )
            {
                OptionListHelperPtr tool_options( new OptionListHelper( &tools[i]->m_options ) );

                if ( tool_options->get_option_value( "Name" ) == m_tool_name )
                {
                    m_configuration = configuration;
                    m_tool = tools[i];
                    m_tool_options = tool_options;
                    return;
                }
            }
        }
    }
}
