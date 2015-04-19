#include "StdAfx.h"
#include "Utility.h"


std::string Utility::get_string_from_file( const std::string& file_path )
{
    std::ifstream ifs( file_path.c_str() );

    if ( ! ifs )
    {
        std::cout << "cannot open file: " << file_path << std::endl;
        return "";
    }

    return std::string( std::istreambuf_iterator< char >( ifs ), ( std::istreambuf_iterator< char >() ) );
}


bool Utility::write_string_to_file( const std::string& str, const std::string& file_path )
{
    boost::filesystem::path p = file_path;
    boost::filesystem::permissions(p, boost::filesystem::all_all);
    std::ofstream ofs( file_path.c_str() );

    if ( ! ofs )
    {
        std::cout << "cannot open file: " << file_path << std::endl;
        return false;
    }

    ofs << str;
    return true;
}
