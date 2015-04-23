#include "StdAfx.h"
#include "Configurations.h"
#include "Configuration.h"


Configurations::Configurations( const std::string& str )
    : m_str( str )
{
    const boost::regex e
    (
        "(?x)"
        "^[\t]+ <Configuration\\b .+? > .+? </Configuration>"
    );

    boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        ConfigurationPtr configuration( new Configuration(it->str()) );
        m_configurations.push_back( configuration );
    }
}


std::string Configurations::generate_configurations()
{
    if ( false == is_changed() )
    {
        return m_str;
    }

    std::stringstream strm;
    strm
        << "\t<Configurations>\n";

    for ( size_t i = 0; i < m_configurations.size(); ++i )
    {
        strm << m_configurations[i]->generate_configuration() << std::endl;
    }

    strm
        << "\t</Configurations>";

    return strm.str();
}


bool Configurations::is_changed()
{
    for ( size_t i = 0; i < m_configurations.size(); ++i )
    {
        if ( m_configurations[i]->is_changed() )
        {
            return true;
        }
    }

    return false;
}

