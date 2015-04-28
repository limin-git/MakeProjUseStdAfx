#pragma once
#include "CommonDefs.h"


struct Utility
{
    static std::string get_string_from_file( const path& p, size_t lines = 0 );
    static bool write_string_to_file( const std::string& str, const path& file );
    static OptionList extract_options_from_string( const std::string& str );
    static std::ostream& output_options( std::ostream& os, const OptionList& options, const std::string& indent = "" );
    static std::ostream& output_paths( std::ostream& os, std::vector<path>& paths );
    static path search_parent_relative_path( const path& current_path, const path& parent_path, size_t deepth = 10 );
    static path search_StdAfx_pch_relative_path( const path& current_path, const std::string& configuration_name = "Debug", size_t deepth = 10 );
    static const boost::regex& create_regex( const char* str );
};
