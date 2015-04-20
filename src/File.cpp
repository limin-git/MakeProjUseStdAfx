#include "StdAfx.h"
#include "File.h"
#include "Utility.h"
#include "FileConfiguration.h"


File::File( const std::string& str )
    : m_str( str )
{
    {
        static const boost::regex e
        (
            "(?x)"
            "<File .+? >"
        );

        boost::smatch m;

        if ( boost::regex_search( m_str, m, e ) )
        {
            m_options = Utility::extract_options_from_string( m.str() );
            //Utility::output_options( std::cout, m_options, "\t\t\t" );
        }
    }

    {
        static const boost::regex e
        (
            "(?x)"
            "^[\t]+ <FileConfiguration .+? >"
            ".+?"
            "^[\t]+ </FileConfiguration>"
        );

        boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
        boost::sregex_iterator end;

        for ( ; it != end; ++it )
        {
            //std::cout << it->str() << std::endl;
            FileConfigurationPtr file_configuration( new FileConfiguration(it->str()) );
            m_file_configurations.push_back( file_configuration );
        }
    }
}
