#include "StdAfx.h"
#include "Utility.h"


std::string Utility::get_string_from_file( const path& file_path )
{
    std::ifstream ifs( file_path.string().c_str() );

    if ( ! ifs )
    {
        std::cout << "\t" << "cannot open file: " << file_path << std::endl;
        return "";
    }

    return std::string( std::istreambuf_iterator< char >( ifs ), ( std::istreambuf_iterator< char >() ) );
}


bool Utility::write_string_to_file( const std::string& str, const path& file_path )
{
    if ( boost::filesystem::exists( file_path ) )
    {
        boost::filesystem::path p = file_path;
        boost::system::error_code ec;
        boost::filesystem::permissions(p, boost::filesystem::all_all, ec);

        if ( ec )
        {
            std::cout << "\t" << "cannot acquire permissions for " << file_path.string() << ": " << ec.message() << "\n";
            return false;
        }
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


std::ostream& Utility::output_paths( std::ostream& os, std::vector<path>& paths )
{
    for ( size_t i = 0; i < paths.size(); ++i )
    {
        os << paths[i].string() << std::endl;
    }

    return os;
}


path Utility::search_parent_relative_path( const path& current_path, const path& parent_path, size_t deepth )
{
    // current: c:\code\transactive\core\corba
    // parent:  c:\code\cots
    // return:  ..\..\.., it means c:\code\cots = c:\code\transactive\core\corba\..\..\..\cots

    path relative_path;

    for ( size_t i = 0; i < deepth; ++i )
    {
        if ( boost::filesystem::exists( current_path / relative_path / parent_path ) )
        {
            return relative_path;
        }

        relative_path /= "..";
    }

    return path();
}
