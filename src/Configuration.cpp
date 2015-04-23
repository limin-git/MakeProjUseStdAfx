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

        const boost::regex e
        (
            "(?x)"
            "<Configuration .+? >"
        );

        boost::smatch m;

        if ( boost::regex_search( m_str, m, e ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        // Tool

        const boost::regex e
        (
            "(?x)"
            "^[\t]+ <Tool .+? />"
        );

        boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
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

    size_t tabs = 0;
    while ( m_str[tabs++] == '\t' );
    std::string indent = std::string( tabs - 1, '\t' );
    std::string next_indent = indent + '\t';

    std::stringstream strm;
    strm << indent << "<Configuration\n";
    Utility::output_options( strm, m_options, next_indent );
    strm << next_indent << ">\n";

    for ( size_t i = 0; i < m_tools.size(); ++i )
    {
        strm << m_tools[i]->generate_tool() << std::endl;
    }

    strm << indent << "</Configuration>";

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
