#include "StdAfx.h"
#include "Tool.h"
#include "Utility.h"


Tool::Tool( const std::string& str )
    : m_str( str )
{
    m_options = Utility::extract_options_from_string( m_str );
    //Utility::output_options( std::cout, m_options );
}
