#include "StdAfx.h"
#include "VCPreBuildEventToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"
#include "Utility.h"


void VCPreBuildEventToolMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    ToolMaker::initialize( project, configuration_name );

    if ( ! m_configuration )
    {
        return;
    }

    make_CommandLine();

    if ( m_tool_options->is_changed() )
    {
        m_tool->set_changed();
    }
}


void VCPreBuildEventToolMaker::make_CommandLine()
{
    const std::string enter_line = "&#x0D;&#x0A;";
    const std::string option_name = "CommandLine";
    const std::string& option_value = m_tool_options->get_option_value( option_name );

    if ( boost::regex_search( option_value, Utility::create_regex( "XCOPY /D /Y /F.*?vc90" ) ) )
    {
        return;
    }

    path stdafx_relative_path = Utility::search_StdAfx_pch_relative_path( m_project->m_current_path, m_configuration_name );

    if ( stdafx_relative_path.empty() )
    {
        std::cout << "\t" << "can not find " << StdAfx_pch << std::endl;
        return;
    }

    std::stringstream option_value_strm;
    option_value_strm << "XCOPY /D /Y /F " << stdafx_relative_path.string() << "\\$(ConfigurationName)\\vc90.?db $(IntDir)\\";

    if ( false == m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->insert_option( option_name, option_value_strm.str(), OptionListHelper::After, "Name" );
    }
    else if ( option_value.empty() )
    {
        m_tool_options->modify_option( option_name, option_value_strm.str() );
    }
    else
    {
        m_tool_options->modify_option( option_name, option_value + enter_line + option_value_strm.str() );
    }

    remove_vc90_xdb();
}


void VCPreBuildEventToolMaker::remove_vc90_xdb()
{
    const std::string option_name = "IntermediateDirectory";
    OptionListHelper configuration_options( &m_configuration->m_options );

    if ( false == configuration_options.is_option_exist( option_name ) )
    {
        return;
    }

    const std::string& option_value = configuration_options.get_option_value( option_name );

    if ( option_value.empty() )
    {
        return;
    }

    const path director = boost::filesystem::system_complete( m_project->m_current_path / option_value );
    const path vc90_xdb[] = { director / "vc90.pdb", director / "vc90.idb" };
    const size_t size = sizeof(vc90_xdb)/sizeof(path);

    for ( size_t i = 0; i < size; ++i )
    {
        if ( boost::filesystem::exists( vc90_xdb[i] ) )
        {
            boost::system::error_code ec;
            boost::filesystem::permissions( vc90_xdb[i], all_all );
            boost::filesystem::remove( vc90_xdb[i], ec );

            if ( ec )
            {
                std::cout << "\t" << "can not delete " << vc90_xdb[i].string() << ", error: " << ec.message() << std::endl;
            }
            else
            {
                std::cout << "\t" << "deleted " << vc90_xdb[i].string() << std::endl;
            }
        }
    }
}
