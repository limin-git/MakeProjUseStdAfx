#include "StdAfx.h"
#include "File.h"
#include "Utility.h"


File::File( const std::string& str )
    : m_str( str )
{
    static const boost::regex e
    (
        "(?x)"
        "<File .+? >"
    );

    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        m_options = Utility::extract_options_from_string( m.str() );
        //Utility::output_options( std::cout, m_options, "\t\t\t" );
    }
}
