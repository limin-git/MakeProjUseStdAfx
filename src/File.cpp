#include "StdAfx.h"
#include "File.h"
#include "Utility.h"
#include "FileConfiguration.h"


File::File( const std::string& str )
    : m_str( str )
{
    {
        boost::smatch m;
        const char* e =
            "(?x)"
            "<File .+? >"
            ;

        if ( boost::regex_search( m_str, m, Utility::create_regex(e) ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        const char* e =
            "(?x)"
            "^[\t]+ <FileConfiguration .+? >"
            ".+?"
            "^[\t]+ </FileConfiguration>"
            ;
        boost::sregex_iterator it( m_str.begin(), m_str.end(), Utility::create_regex(e) );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FileConfigurationPtr file_configuration( new FileConfiguration(it->str()) );
            m_file_configurations.push_back( file_configuration );
        }
    }
}
