#include "StdAfx.h"
#include "Configuration.h"
#include "Tool.h"
#include "Utility.h"


Configuration::Configuration( const std::string& str )
    : m_str( str )
{
    {
        // option

        static const boost::regex e
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

        static const boost::regex e
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
    return m_str;
}
