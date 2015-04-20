#include "StdAfx.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "Tool.h"


FileConfiguration::FileConfiguration( const std::string& str )
    : m_str( str )
{
    {
        static const boost::regex e
        (
            "(?x)"
            "<FileConfiguration .+? >"
        );

        boost::smatch m;

        if ( boost::regex_search( m_str, m, e ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        static const boost::regex e
        (
            "(?x)"
            "^[\t]+ <Tool .+? />"
        );

        boost::smatch m;

        if ( boost::regex_search( m_str, m, e ) )
        {
            m_tool.reset( new Tool( m.str() ) );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }
}
