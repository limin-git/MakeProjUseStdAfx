#include "StdAfx.h"
#include "Utility.h"


std::string Utility::get_string_from_file( const std::string& file_path )
{
    std::ifstream ifs( file_path.c_str() );

    if ( ! ifs )
    {
        std::cout << "\t" << "cannot open file: " << file_path << std::endl;
        return "";
    }

    return std::string( std::istreambuf_iterator< char >( ifs ), ( std::istreambuf_iterator< char >() ) );
}


bool Utility::write_string_to_file( const std::string& str, const std::string& file_path )
{
    boost::filesystem::path p = file_path;
    boost::system::error_code ec;
    boost::filesystem::permissions(p, boost::filesystem::all_all, ec);

    if ( ec )
    {
        std::cout << "\t" << ec.message() << "\n";
        return false;
    }

    std::ofstream ofs( file_path.c_str() );

    if ( ! ofs )
    {
        std::cout << "\t" << "cannot open file: " << file_path << std::endl;
        return false;
    }

    ofs << str;
    return true;
}


OptionList Utility::extract_options_from_string( const std::string& str )
{
    OptionList options;

    static const boost::regex e
    (
        "(?x)"
        "( \\w+ )"          //name
        " = "
        "\" ( [^\"]* ) \""  //value
    );

    boost::sregex_iterator it( str.begin(), str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        options.push_back( Option( it->str(1), it->str(2) ) );
    }

    return options;
}


std::ostream& Utility::output_options( std::ostream& os, const OptionList& options, const std::string& indent )
{
    for ( OptionList::const_iterator it = options.begin(); it != options.end(); ++it )
    {
        os << indent << it->first << "=\"" << it->second << "\"" << std::endl;
    }

    return os;
}
