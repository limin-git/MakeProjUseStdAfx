#include "StdAfx.h"
#include "VCPreBuildEventToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"


void VCPreBuildEventToolMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;
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

        if ( configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
        {
            ToolPtrList& tools = configuration->m_tools;

            for ( size_t i = 0; i < tools.size(); ++i )
            {
                OptionListHelperPtr tool_options( new OptionListHelper( &tools[i]->m_options ) );

                if ( tool_options->get_option_value( "Name" ) == "VCPreBuildEventTool" )
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


void VCPreBuildEventToolMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    initialize( project, configuration_name );

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
    const std::string stdafx_pch = "TA_StdAfx.pch";
    const std::string& option_value = m_tool_options->get_option_value( option_name );

    if ( option_value.find( "stdafx" ) != std::string::npos )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    bool is_path_found = false;
    path stdafx_relative_path;

    const char* short_paths[] = { "stdafx", "core\\stdafx", "transactive\\core\\stdafx", "code\\transactive\\core\\stdafx" };
    size_t cnt = sizeof(short_paths) / sizeof(const char*);

    for ( size_t i = 0; i < cnt && false == is_path_found; ++i  )
    {
        stdafx_relative_path = "..";

        for ( size_t j = 0; j < 10; ++j )
        {
            if ( boost::filesystem::exists( current_path / stdafx_relative_path / short_paths[i] / m_configuration_name / stdafx_pch ) )
            {
                stdafx_relative_path = stdafx_relative_path / short_paths[i];
                is_path_found = true;
                break;
            }

            stdafx_relative_path /= "..";
        }
    }

    if ( false == is_path_found )
    {
        std::cout << "\t" << "can not find TA_StdAfx.pch" << std::endl;
        return;
    }

    std::stringstream option_value_strm;
    option_value_strm
        << "XCOPY /D /Y /F " << stdafx_relative_path.string() << "\\$(ConfigurationName)\\vc90.?db $(IntDir)\\" << enter_line
        //<< "IF NOT EXIST $(IntDir)\\vc90.idb COPY " << stdafx_relative_path.string() << "\\$(ConfigurationName)\\vc90.idb $(IntDir)\\" << enter_line
        ;

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

    remove_vc90_pdb_idb();
}


void VCPreBuildEventToolMaker::remove_vc90_pdb_idb()
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

    path director = boost::filesystem::system_complete( m_project->m_current_path / option_value );
    path vc90_pdb = director / "vc90.pdb";
    path vc90_idb = director / "vc90.idb";

    if ( boost::filesystem::exists( vc90_pdb ) )
    {
        boost::system::error_code ec;
        boost::filesystem::permissions(vc90_pdb, all_all);
        boost::filesystem::remove( vc90_pdb, ec);

        if ( ec )
        {
            std::cout << "\t" << "cannot delete " << vc90_pdb.string() << ", error: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "\t" << "deleted " << vc90_pdb.string() << std::endl;
        }
    }

    if ( boost::filesystem::exists( vc90_idb ) )
    {
        boost::system::error_code ec;
        boost::filesystem::permissions(vc90_idb, all_all);
        boost::filesystem::remove( vc90_idb, ec);

        if ( ec )
        {
            std::cout << "\t" << "cannot delete " << vc90_idb.string() << ", error: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "\t" << "deleted " << vc90_idb.string() << std::endl;
        }
    }
}
