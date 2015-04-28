#include "StdAfx.h"
#include "Filter.h"
#include "File.h"
#include "Utility.h"


Filter::Filter( const std::string& str )
    : m_str( str )
{
    {
        // option

        boost::smatch m;
        const char* e =
            "(?x)"
            "(^[\t]+)"
            "<Filter .+? >"
            ;

        if ( boost::regex_search( m_str, m, Utility::create_regex(e) ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            m_indent = m.str(1);
            //Utility::output_options( std::cout, m_options );
        }
    }

    // next level tabs
    size_t tabs = m_indent.size() + 1;

    {
        std::stringstream filter_regex_strm;
        filter_regex_strm
            << "(?x)"
            << "^[\t]{" << tabs << "} <Filter  .+? >"
            << ".+?"
            << "^[\t]{" << tabs << "} </Filter>"
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
            << "^[\t]{" << tabs << "} <File \\b  .+? >"
            << ".+?"
            << "^[\t]{" << tabs << "} </File>"
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


const std::string& Filter::generate_filter()
{
    return m_str;
}
