#include "StdAfx.h"
#include "Filter.h"
#include "File.h"
#include "Utility.h"


Filter::Filter( const std::string& str )
    : m_str( str )
{
    {
        // option

        boost::regex e
        (
            "(?x)"
            "<Filter .+? >"
        );
        boost::smatch m;

        if ( boost::regex_search( m_str, m, e ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options );
        }
    }

    // count next level tabs
    size_t tabs = 0;
    while ( m_str[tabs++] == '\t' );

    {
        std::stringstream filter_regex_strm;
        filter_regex_strm
            << "(?x)"
            << "^[\t]{" << tabs << "} <Filter  .+? >"
            << ".+?"
            << "^[\t]{" << tabs << "} </Filter .+? >"
            ;
        boost::regex filter_regex( filter_regex_strm.str() );
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
        std::stringstream file_regex_strm;
        file_regex_strm
            << "(?x)"
            << "^[\t]{" << tabs << "} <File  .+? >"
            << ".+?"
            << "^[\t]{" << tabs << "} </File .+? >"
            ;
        boost::regex file_regex( file_regex_strm.str() );
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
