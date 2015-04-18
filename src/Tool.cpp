#include "StdAfx.h"
#include "Tool.h"
#include "Vcproj.h"


Tool::Tool( Vcproj* project, const std::string& str, size_t pos )
    : m_project( project ),
      m_str( str ),
      m_pos( pos ),
      m_changed( false )
{
    static const boost::regex e
    (
        "(?x)"
        "( \\w+ )"          //name
        " = "
        "\" ( [^\"]* ) \""  //value
    );

    boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        m_options.push_back( std::make_pair( it->str(1), it->str(2) ) );
    }

    boost::smatch m;

    if ( boost::regex_search( m_str, m, boost::regex( "(?x) Tool \\n (\\s+) Name" ) ) )
    {
        m_indent_str = m.str(1);
    }
}


std::ostream& Tool::output_options( std::ostream& os )
{
    for ( OptionList::const_iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        os << m_indent_str << it->first << "=\"" << it->second << "\"" << std::endl;
    }

    return os;
}


Option& Tool::find_option( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        if ( it->first == option_name )
        {
            return *it;
        }
    }

    static Option empty;
    return empty;
}


std::string Tool::make_tool()
{
    std::stringstream strm;
    strm << "<Tool" << std::endl;
    output_options( strm );
    strm << m_indent_str.substr( 0, m_indent_str.size() -1 ) << "/>";
    return strm.str();
}


void Tool::save_tool()
{
    if ( m_changed )
    {
        m_project->m_str.replace( m_pos, m_str.size(), make_tool() );
    }
}
