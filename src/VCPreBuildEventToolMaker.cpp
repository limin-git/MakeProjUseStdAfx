#include "StdAfx.h"
#include "VCPreBuildEventToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"


VCPreBuildEventToolMaker::VCPreBuildEventToolMaker( VisualStudioProjectPtr project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
    std::vector<ConfigurationPtr>& configurations = m_project->m_configurations->m_configurations;
    for ( size_t i = 0; i < configurations.size(); ++i )
    {
        ConfigurationPtr configuration = configurations[i];
        OptionListHelperPtr config_opt_hlpr( new OptionListHelper( &configuration->m_options ) );
        const Option& option = config_opt_hlpr->get_option( "Name" );
        if ( option.second == ( m_configuration_name + "|Win32" ) )
        {
            std::vector<ToolPtr>& tools = configuration->m_tools;
            for ( size_t i = 0; i < tools.size(); ++i )
            {
                OptionListHelperPtr tool_opt_hlpr( new OptionListHelper( &tools[i]->m_options ) );
                const Option& option = tool_opt_hlpr->get_option( "Name" );
                if ( "VCPreBuildEventTool" == option.second )
                {
                    m_configuration = configuration;
                    m_tool = tools[i];
                    m_helper = tool_opt_hlpr;
                    return;
                }
            }
        }
    }
}


void VCPreBuildEventToolMaker::make_all()
{
    make_CommandLine();

    if ( m_helper->is_changed() )
    {
        m_tool->set_changed();
    }
}


void VCPreBuildEventToolMaker::make_CommandLine()
{
    const char* enter_line = "&#x0D;&#x0A;";
    const char* option_name = "CommandLine";
    const Option& option = m_helper->get_option( option_name );

    if ( option.second.find( "stdafx" ) != std::string::npos )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    path stdafx_pch_path = "..";
    bool is_path_found = false;

    for ( size_t i = 0; i < 10; ++i )
    {
        if ( boost::filesystem::exists( current_path / stdafx_pch_path / "stdafx" / m_configuration_name / "TA_StdAfx.pch" ) )
        {
            is_path_found = true;
            stdafx_pch_path.make_preferred();
            break;
        }

        stdafx_pch_path = ".." / stdafx_pch_path;
    }

    if ( false == is_path_found )
    {
        std::cout << "\t" << "can not find TA_StdAfx.pch" << std::endl;
        return;
    }

    std::stringstream option_value_strm;
    option_value_strm
        << "IF NOT EXIST $(IntDir)\\vc90.pdb COPY " << stdafx_pch_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.pdb $(IntDir)\\vc90.pdb" << enter_line
        << "IF NOT EXIST $(IntDir)\\vc90.idb COPY " << stdafx_pch_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.idb $(IntDir)\\vc90.idb" << enter_line
        ;

    if ( false == m_helper->is_option_exist( option_name ) )
    {
        m_helper->insert_option( option_name, option_value_strm.str(), OptionListHelper::After, "Name" );
    }
    else if ( option.second.empty() )
    {
        m_helper->modify_option( option_name, option_value_strm.str() );
    }
    else
    {
        m_helper->modify_option( option_name, option.second + enter_line + option_value_strm.str() );
    }

    remove_vc90_pdb_idb();
}


void VCPreBuildEventToolMaker::remove_vc90_pdb_idb()
{
    const std::string option_name = "IntermediateDirectory";
    OptionListHelperPtr helper( new OptionListHelper( &m_configuration->m_options ) );

    if ( false == helper->is_option_exist( option_name ) )
    {
        return;
    }

    const Option& option = helper->get_option( option_name );

    if ( option.second.empty() )
    {
        return;
    }

    path director = boost::filesystem::system_complete( m_project->m_current_path / option.second );
    path vc90_pdb = director / "vc90.pdb";
    path vc90_idb = director / "vc90.idb";

    if ( boost::filesystem::exists( vc90_pdb ) )
    {
        boost::system::error_code ec;
        permissions(vc90_pdb, all_all);
        remove( vc90_pdb, ec);

        if ( ec )
        {
            std::cout << "\t" << "cannot delete " << vc90_pdb.string() << ", error: " << ec.message() << std::endl;
        }
    }

    if ( boost::filesystem::exists( vc90_idb ) )
    {
        boost::system::error_code ec;
        permissions(vc90_idb, all_all);
        remove( vc90_idb, ec);

        if ( ec )
        {
            std::cout << "\t" << "cannot delete " << vc90_idb.string() << ", error: " << ec.message() << std::endl;
        }
    }
}
