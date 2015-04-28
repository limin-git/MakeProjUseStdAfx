#include "StdAfx.h"
#include "Utility.h"


std::string Utility::get_string_from_file( const path& p, size_t lines )
{
    std::ifstream ifs( p.string().c_str() );

    if ( ! ifs )
    {
        std::cout << "\t" << "can not open file: " << p << std::endl;
        return "";
    }

    if ( 0 == lines)
    {
        return std::string( std::istreambuf_iterator< char >( ifs ), ( std::istreambuf_iterator< char >() ) );
    }
    else
    {
        std::string line;
        std::stringstream strm;

        while ( lines-- )
        {
            if ( ! std::getline( ifs, line ) )
            {
                break;
            }

            strm << line << std::endl;
        }

        return strm.str();
    }
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
            std::cout << "\t" << "can not acquire permissions for " << file_path.string() << ": " << ec.message() << "\n";
            return false;
        }
    }

    std::ofstream ofs( file_path.c_str() );

    if ( ! ofs )
    {
        std::cout << "\t" << "can not open file: " << file_path << std::endl;
        return false;
    }

    ofs << str;
    return true;
}


OptionList Utility::extract_options_from_string( const std::string& str )
{
    OptionList options;
    const char* e =
        "(?x)"
        "( \\w+ )"          // $1: name
        " = "
        "\" ( [^\"]* ) \""  // $2: value
        ;
    boost::sregex_iterator it( str.begin(), str.end(), create_regex(e) );
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
    // Example:
    //     current: c:\code\transactive\core\corba
    //     parent:  c:\code\cots
    //     return:  ..\..\.., it means c:\code\cots = c:\code\transactive\core\corba\..\..\..\cots
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


path Utility::search_StdAfx_pch_relative_path( const path& current_path, const std::string& configuration_name, size_t deepth )
{
    const path short_paths[] = { "stdafx", "core\\stdafx", "transactive\\core\\stdafx", "code\\transactive\\core\\stdafx" };
    const size_t size = sizeof(short_paths) / sizeof(path);

    for ( size_t i = 0; i < size; ++i  )
    {
        path stdafx_relative_path;

        for ( size_t j = 0; j < deepth; ++j )
        {
            if ( boost::filesystem::exists( current_path / stdafx_relative_path / short_paths[i] / configuration_name / StdAfx_pch ) )
            {
                return stdafx_relative_path / short_paths[i];
            }

            stdafx_relative_path /= "..";
        }
    }

    return path();
}


const boost::regex& Utility::create_regex( const char* s )
{
    static boost::mutex s_mutex;
    static std::map<const char*, boost::regex> s_regex_map;

    boost::lock_guard<boost::mutex> lock(s_mutex);
    std::map<const char*, boost::regex>::iterator it = s_regex_map.find(s);

    if ( it == s_regex_map.end() )
    {
        it = s_regex_map.insert( std::make_pair( s, boost::regex(s) ) ).first;
    }

    return it->second;
}
