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


bool Tool::is_option_exist( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        if ( it->first == option_name )
        {
            return true;
        }
    }

    return false;
}


std::string Tool::generate_tool_string()
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
        m_project->m_str.replace( m_pos, m_str.size(), generate_tool_string() );
    }
}


void Tool::modify_option( const std::string& option_name, const std::string& option_value )
{
    Option& option = find_option( option_name );

    if ( option.first.empty() )
    {
        std::cout << "cannot modify option " << option_name << std::endl;
        return;
    }

    if ( option.second != option_value )
    {
        option.second = option_value;
        m_changed = true;
        std::cout << option_name << ": +- " << option_value << std::endl;
    }
}


void Tool::insert_option( const std::string& option_name, const std::string& option_value, ops_type pos, const std::string& option )
{
    if ( false == find_option( option_name ).first.empty() )
    {
        std::cout << option_name << " already exist." << std::endl;
        return;
    }

    for ( OptionList::iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        if ( it->first == option )
        {
            if ( BEFORE == pos )
            {
                m_options.insert( it, std::make_pair(option_name, option_value) );
            }
            else
            {
                m_options.insert( ++it, std::make_pair(option_name, option_value) );
            }

            m_changed = true;
            std::cout << "+ " << option_name << ": " << option_value << std::endl;
            return;
        }
    }

    std::cout << "cannot insert option " << option_name << " with value " << option_value << ( AFTER == pos ? " after " : " before ") << " option " << option << std::endl;
    m_options.push_back( std::make_pair(option_name, option_value) );
    m_changed = true;
    std::cout << "+ " << option_name << ": " << option_value << std::endl;
}


void Tool::remove_option( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options.begin(); it != m_options.end(); ++it )
    {
        if ( it->first == option_name )
        {
            std::cout << "- " << option_name << ": " << it->second << std::endl;
            m_options.erase( it );
            m_changed = true;
            return;
        }
    }
}
