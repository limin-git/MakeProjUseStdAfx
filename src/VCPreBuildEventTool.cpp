#include "StdAfx.h"
#include "VCPreBuildEventTool.h"
#include "Vcproj.h"


void VCPreBuildEventTool::make_tool()
{
    make_CommandLine();
    save_tool();
}


void VCPreBuildEventTool::make_CommandLine()
{
    const char* enter_line = "&#x0D;&#x0A;";
    const char* option_name = "CommandLine";
    // IF EXIST $(IntDir)\vc90.pdb DEL /Q /F $(IntDir)\vc90.pdb&#x0D;&#x0A;
    // IF EXIST $(IntDir)\vc90.idb  DEL /Q /F $(IntDir)\vc90.idb&#x0D;&#x0A;
    // MKLINK /H $(IntDir)\vc90.pdb ..\StdAfx\$(ConfigurationName)\vc90.pdb&#x0D;&#x0A;
    // MKLINK /H $(IntDir)\vc90.idb  ..\StdAfx\$(ConfigurationName)\vc90.idb&#x0D;&#x0A;

    Option& option = find_option( option_name );
    std::string& option_value = option.second;

    if ( option_value.find( "stdafx" ) != std::string::npos )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    std::string configuration_type = m_project->m_configuration_name;
    path stdafx_relative_path = "..";
    bool is_found_path = false;

    for ( size_t i = 0; i < 20; ++i )
    {
        if ( boost::filesystem::exists( current_path / stdafx_relative_path / "stdafx" / configuration_type / "TA_StdAfx.pch" ) )
        {
            is_found_path = true;
            break;
        }

        stdafx_relative_path = ".." / stdafx_relative_path;
    }

    if ( false == is_found_path )
    {
        std::cout << "\t" << "can not find TA_StdAfx.pch" << std::endl;
        return;
    }

    std::stringstream strm;
    strm
        << "IF NOT EXIST $(IntDir)\\vc90.pdb COPY " << stdafx_relative_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.pdb $(IntDir)\\vc90.pdb" << enter_line
        << "IF NOT EXIST $(IntDir)\\vc90.idb COPY " << stdafx_relative_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.idb $(IntDir)\\vc90.idb" << enter_line
        ;
    if ( option.first.empty() )
    {
        m_options.push_back( std::make_pair( option_name, strm.str() ) );
        m_changed = true;
        std::cout << "\t" << "+ " << option_name << ": " << strm.str() << std::endl;
    }
    else if ( option.second.empty() )
    {
        option_value = strm.str();
        m_changed = true;
        std::cout << "\t" << option_name << ": + " << strm.str() << std::endl;
    }
    else
    {
        option_value += enter_line;
        m_changed = true;
        std::cout << "\t" << option_name << ": + " << strm.str() << std::endl;
    }
}
