#include "StdAfx.h"
#include "Files.h"
#include "Filter.h"
#include "File.h"
#include "Utility.h"


Files::Files( const std::string& str )
    : m_str( str ),
      m_is_changed( false )
{
    {
        const char* e =
            "(?x)"
            "^[\t]{2} <Filter\\b .+? >"
            ".+?"
            "^[\t]{2} </Filter>"
            ;
        boost::sregex_iterator it( m_str.begin(), m_str.end(), Utility::create_regex(e) );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FilterPtr filter( new Filter(it->str()) );
            m_filters.push_back( filter );
        }
    }

    {
        const char* e =
            "(?x)"
            "^[\t]{2} <File \\b .+? >"
            ".+?"
            "^[\t]{2} </File>"
            ;
        boost::sregex_iterator it( m_str.begin(), m_str.end(), Utility::create_regex(e) );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FilePtr file( new File(it->str()) );
            m_files.push_back( file );
        }
    }
}


const std::string& Files::generate_files()
{
    return m_str;
}
