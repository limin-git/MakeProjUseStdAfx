#include "StdAfx.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "Tool.h"


FileConfiguration::FileConfiguration( const std::string& str )
    : m_str( str )
{
    {
        boost::smatch m;
        const char* e =
            "(?x)"
            "<FileConfiguration .+? >"
            ;

        if ( boost::regex_search( m_str, m, Utility::create_regex(e) ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        boost::smatch m;
        const char* e =
            "(?x)"
            "^[\t]+ <Tool .+? />"
            ;

        if ( boost::regex_search( m_str, m, Utility::create_regex(e) ) )
        {
            m_tool.reset( new Tool( m.str() ) );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }
}
