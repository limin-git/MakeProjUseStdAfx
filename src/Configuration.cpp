#include "StdAfx.h"
#include "Configuration.h"
#include "Tool.h"
#include "Utility.h"


Configuration::Configuration( const std::string& str )
    : m_str( str ),
      m_is_changed( false )
{
    {
        // option

        boost::smatch m;
        const char* e =
            "(?x)"
            "^([\t]+)"
            "<Configuration .+? >"
            ;

        if ( boost::regex_search( m_str, m, Utility::create_regex(e) ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            m_indent = m.str(1);
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        // Tool

        const char* e =
            "(?x)"
            "^[\t]+ <Tool .+? />"
            ;
        boost::sregex_iterator it( m_str.begin(), m_str.end(), Utility::create_regex(e) );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            ToolPtr tool( new Tool(it->str()) );
            m_tools.push_back( tool );
        }
    }
}


std::string Configuration::generate_configuration()
{
    if ( false == is_changed() )
    {
        return m_str;
    }

    std::stringstream strm;

    strm << m_indent << "<Configuration\n";
    Utility::output_options( strm, m_options, m_indent + "\t" );
    strm << m_indent << "\t>\n";
    
    for ( size_t i = 0; i < m_tools.size(); ++i )
    {
        strm << m_tools[i]->generate_tool() << std::endl;
    }

    strm << m_indent << "</Configuration>";

    return strm.str();
}


bool Configuration::is_changed()
{
    if ( m_is_changed )
    {
        return true;
    }

    for ( size_t i = 0; i < m_tools.size(); ++i )
    {
        if ( m_tools[i]->is_changed() )
        {
            return true;
        }
    }

    return false;
}
