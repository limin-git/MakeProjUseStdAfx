#include "StdAfx.h"
#include "VCPreBuildEventTool.h"
#include "Vcproj.h"


VCPreBuildEventTool::VCPreBuildEventTool( Vcproj* project, std::string& proj_str, const std::string& str, size_t pos )
    : m_project( project ),
      m_project_string( proj_str ),
      m_string( str ),
      m_pos( pos ),
      m_is_changed( false )
{
    static const boost::regex e
    (
        "(?x)"
        "( \\w+ )"          //name
        " = "
        "\" ( [^\"]* ) \""  //value
    );

    boost::sregex_iterator it( m_string.begin(), m_string.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        m_options.push_back( std::make_pair( it->str(1), it->str(2) ) );
    }

    boost::smatch m;

    if ( boost::regex_search( m_string, m, boost::regex( "(?x) Tool \\n (\\s+) Name" ) ) )
    {
        m_indent_string = m.str(1);
    }
}


std::ostream& VCPreBuildEventTool::output( std::ostream& os, const OptionsType& options, const std::string& indent )
{
    for ( OptionsType::const_iterator it = options.begin(); it != options.end(); ++it )
    {
        os << indent << it->first << "=\"" << it->second << "\"" << std::endl;
    }

    return os;
}


OptionType& VCPreBuildEventTool::find_option( const std::string& option_name )
{
    for ( OptionsType::iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        if ( it->first == option_name )
        {
            return *it;
        }
    }

    static OptionType empty;
    return empty;
}


std::string VCPreBuildEventTool::make_tool()
{
    std::stringstream strm;
    strm << "<Tool" << std::endl;
    output( strm, m_options, m_indent_string );
    strm << m_indent_string.substr( 0, m_indent_string.size() -1 ) << "/>";
    return strm.str();
}


void VCPreBuildEventTool::save_tool()
{
    if ( m_is_changed )
    {
        m_project_string.replace( m_pos, m_string.size(), make_tool() );
    }
}


void VCPreBuildEventTool::make_CommandLine()
{
    const char* ENTER_LINE = "&#x0D;&#x0A;";
    // IF EXIST $(IntDir)\vc90.pdb DEL /Q /F $(IntDir)\vc90.pdb&#x0D;&#x0A;
    // IF EXIST $(IntDir)\vc90.idb  DEL /Q /F $(IntDir)\vc90.idb&#x0D;&#x0A;
    // MKLINK /H $(IntDir)\vc90.pdb ..\StdAfx\$(ConfigurationName)\vc90.pdb&#x0D;&#x0A;
    // MKLINK /H $(IntDir)\vc90.idb  ..\StdAfx\$(ConfigurationName)\vc90.idb&#x0D;&#x0A;


    OptionType& option = find_option( "CommandLine" );
    std::string& option_value = option.second;

    if ( option_value.find( "stdafx" ) != std::string::npos )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    std::string configuration_type = m_project->m_configuration_type;
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
        std::cout << "can not find TA_StdAfx.pch" << std::endl;
        return;
    }

    std::stringstream strm;
    strm 
        << "IF EXIST $(IntDir)\\vc90.pdb DEL /Q /F $(IntDir)\\vc90.pdb" << ENTER_LINE
        << "IF EXIST $(IntDir)\\vc90.idb DEL /Q /F $(IntDir)\\vc90.idb" <<ENTER_LINE
        << "MKLINK /H $(IntDir)\\vc90.pdb " << stdafx_relative_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.pdb" << ENTER_LINE
        << "MKLINK /H $(IntDir)\\vc90.idb " << stdafx_relative_path.string() << "\\stdafx\\$(ConfigurationName)\\vc90.idb" << ENTER_LINE
        ;

    if ( option.first.empty() )
    {
        m_options.push_back( std::make_pair( "CommandLine", strm.str() ) );
        m_is_changed = true;
        std::cout << "+ CommandLine: " << strm.str() << std::endl;
    }
    else if ( option.second.empty() )
    {
        option_value = strm.str();
        m_is_changed = true;
        std::cout << "CommandLine: + " << strm.str() << std::endl;
    }
    else
    {
        option_value += ENTER_LINE;
        m_is_changed = true;
        std::cout << "CommandLine: + " << strm.str() << std::endl;
    }
}
