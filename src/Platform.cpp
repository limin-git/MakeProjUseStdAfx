#include "StdAfx.h"
#include "Platform.h"
#include "Utility.h"


Platform::Platform( const std::string& str )
    : m_str( str )
{
    m_options = Utility::extract_options_from_string( m_str );
    //Utility::output_options( std::cout, m_options, "\t\t\t");
}


std::string Platform::generate_platform()
{
    return m_str;
}
