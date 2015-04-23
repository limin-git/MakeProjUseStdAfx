#include "StdAfx.h"
#include "Platforms.h"
#include "Platform.h"


Platforms::Platforms( const std::string& str )
    : m_str( str )
{
    const boost::regex e
    (
        "(?x)"
        "^[\t]+ <Platform\\b .+? />"
    );

    boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        PlatformPtr platform( new Platform(it->str()) );
        m_platforms.push_back( platform );
    }
}


std::string Platforms::generate_platforms()
{
    return m_str;
}
