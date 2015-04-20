#include "StdAfx.h"
#include "Tool.h"
#include "Utility.h"


Tool::Tool( const std::string& str )
    : m_str( str ),
      m_is_changed( false )
{
    m_options = Utility::extract_options_from_string( m_str );
    //Utility::output_options( std::cout, m_options );
}


std::string Tool::generate_tool()
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
    strm << indent << "<Tool\n";
    Utility::output_options( strm, m_options, next_indent );
    strm << indent << "/>";

    return strm.str();
}
