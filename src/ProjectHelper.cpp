#include "StdAfx.h"
#include "ProjectHelper.h"
#include "VisualStudioProject.h"
#include "Files.h"
#include "File.h"
#include "Filter.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "Configurations.h"
#include "Configuration.h"
#include "OptionListHelper.h"
#include "Tool.h"


ProjectHelper::ProjectHelper( VisualStudioProject* project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
}


const std::string& ProjectHelper::get_configuration_type()
{
    if ( m_project && m_project->m_configurations )
    {
        ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

        for ( size_t i = 0; i < configurations.size(); ++i )
        {
            OptionListHelper configuration_options( &configurations[i]->m_options );

            if ( configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
            {
                // 0: MakeFile
                // 1: Application (.exe)
                // 2: Dynamic Library (.dll)
                // 4: Static Library (.lib)
                //10: Utility
                return configuration_options.get_option_value( "ConfigurationType" );
            }
        }
    }

    static std::string empty;
    return empty;
}


const std::vector<path>& ProjectHelper::get_additional_include_directories()
{
    if ( false == m_additional_include_directories.empty() )
    {
        return m_additional_include_directories;
    }

    OptionListHelperPtr m_tool_options;

    ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

    for ( size_t i = 0; i < configurations.size(); ++i )
    {
        ConfigurationPtr configuration = configurations[i];
        OptionListHelper configuration_options( &configuration->m_options );

        if ( configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
        {
            ToolPtrList& tools = configuration->m_tools;

            for ( size_t i = 0; i < tools.size(); ++i )
            {
                OptionListHelperPtr tool_options( new OptionListHelper( &tools[i]->m_options ) );

                if ( tool_options->get_option_value( "Name" ) == "VCCLCompilerTool" )
                {
                    m_tool_options = tool_options;
                    break;
                }
            }
        }
    }

    if ( m_tool_options )
    {
        const std::string& additional_directories = m_tool_options->get_option_value( "AdditionalIncludeDirectories" );

        typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep( ";," );
        tokenizer tokens( additional_directories, sep );

        for ( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
        {
            path p(*it);
            //std::cout << p.string() << std::endl;
            p.make_preferred();
            m_additional_include_directories.push_back( p );
        }
    }

    return m_additional_include_directories;
}
