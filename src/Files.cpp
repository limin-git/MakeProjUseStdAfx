#include "StdAfx.h"
#include "Files.h"
#include "Filter.h"
#include "File.h"


Files::Files( const std::string& str )
    : m_str( str )
{
    {
        static const boost::regex filter_regex
        (
            "(?x)"
            "^[\t]{2} <Filter .+? >"
            ".+?"
            "^[\t]{2} </Filter>"
        );

        boost::sregex_iterator it( m_str.begin(), m_str.end(), filter_regex );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FilterPtr filter( new Filter(it->str()) );
            m_filters.push_back( filter );
        }
    }

    {
        static const boost::regex file_regex
        (
            "(?x)"
            "^[\t]{2} <File .+? >"
            ".+?"
            "^[\t]{2} </File>"
        );

        boost::sregex_iterator it( m_str.begin(), m_str.end(), file_regex );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FilePtr file( new File(it->str()) );
            m_files.push_back( file );
        }
    }
}


std::string Files::generate_files()
{
    return m_str;
}
