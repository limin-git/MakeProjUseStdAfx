#include "StdAfx.h"
#include "PreferredPathMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "OptionListHelper.h"
#include "Tool.h"


PreferredPathMaker::PreferredPathMaker( VisualStudioProjectPtr project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
    std::vector<ConfigurationPtr>& configurations = m_project->m_configurations->m_configurations;
    for ( size_t i = 0; i < configurations.size(); ++i )
    {
        OptionListHelperPtr configuration_options( new OptionListHelper( &configurations[i]->m_options ) );
        if ( configuration_options->get_option( "Name" ).second == ( m_configuration_name + "|Win32" ) )
        {
            m_configuration = configurations[i];
            m_configuration_options = configuration_options;

            for ( size_t j = 0; j < configurations[i]->m_tools.size(); ++j )
            {
                OptionListHelperPtr tool_options( new OptionListHelper( &configurations[i]->m_tools[j]->m_options ) );

                if ( tool_options->get_option( "Name" ).second == "VCCLCompilerTool" )
                {
                    m_VCCLCompilerTool = configurations[i]->m_tools[j];
                    m_VCCLCompilerTool_options = tool_options;
                }
                else if ( tool_options->get_option( "Name" ).second == "VCLibrarianTool" )
                {
                    m_VCLibrarianTool = configurations[i]->m_tools[j];
                    m_VCLibrarianTool_options = tool_options;
                }
                else if ( tool_options->get_option( "Name" ).second == "VCLinkerTool" )
                {
                    m_VCLinkerTool = configurations[i]->m_tools[j];
                    m_VCLinkerTool_options = tool_options;
                }
                else if ( tool_options->get_option( "Name" ).second == "VCBscMakeTool" )
                {
                    m_VCBscMakeTool = configurations[i]->m_tools[j];
                    m_VCBscMakeTool_options = tool_options;
                }
            }
        }
    }
}


void PreferredPathMaker::make_all()
{
    make_preferred_path();
}


void PreferredPathMaker::make_preferred_path()
{
    if ( m_configuration_options )
    {
        make_preferred_path_4_option( m_configuration_options, "OutputDirectory" );
        make_preferred_path_4_option( m_configuration_options, "IntermediateDirectory" );
        make_preferred_path_4_option( m_configuration_options, "InheritedPropertySheets" );

        if ( m_configuration_options->is_changed() )
        {
            m_configuration->set_changed();
        }
    }

    if ( m_VCCLCompilerTool_options )
    {
        make_preferred_path_4_option( m_VCCLCompilerTool_options, "AdditionalIncludeDirectories" );
        make_preferred_path_4_option( m_VCCLCompilerTool_options, "PrecompiledHeaderFile" );
        make_preferred_path_4_option( m_VCCLCompilerTool_options, "AssemblerListingLocation" );
        make_preferred_path_4_option( m_VCCLCompilerTool_options, "ObjectFile" );
        make_preferred_path_4_option( m_VCCLCompilerTool_options, "ProgramDataBaseFileName" );

        if ( m_VCCLCompilerTool_options->is_changed() )
        {
            m_VCCLCompilerTool->set_changed();
        }
    }


    if ( m_VCLibrarianTool_options )
    {
        make_preferred_path_4_option( m_VCLibrarianTool_options, "OutputFile" );

        if ( m_VCLibrarianTool_options->is_changed() )
        {
            m_VCLibrarianTool->set_changed();
        }
    }

    if ( m_VCLinkerTool_options )
    {
        make_preferred_path_4_option( m_VCLinkerTool_options, "OutputFile" );
        make_preferred_path_4_option( m_VCLinkerTool_options, "AdditionalLibraryDirectories" );

        if ( m_VCLinkerTool_options->is_changed() )
        {
            m_VCLinkerTool->set_changed();
        }
    }

    if ( m_VCBscMakeTool_options )
    {
        make_preferred_path_4_option( m_VCBscMakeTool_options, "OutputFile" );

        if ( m_VCBscMakeTool_options->is_changed() )
        {
            m_VCBscMakeTool->set_changed();
        }
    }
}


void PreferredPathMaker::make_preferred_path_4_option( OptionListHelperPtr options, const std::string& option_name )
{
    if ( ! options )
    {
        return;
    }

    if ( false == options->is_option_exist( option_name ) )
    {
        return;
    }

    const Option& option = options->get_option( option_name );

    std::string option_value = option.second;
    char* p = const_cast<char*>( option_value.c_str() );
    bool is_changed = false;

    for ( size_t i = 0; i < option_value.size(); ++i, ++p )
    {
        if ( *p == '/' )
        {
            *p = '\\';
            is_changed = true;
        }
    }

    if ( true == is_changed )
    {
        options->modify_option( option_name, option_value );
    }
}
