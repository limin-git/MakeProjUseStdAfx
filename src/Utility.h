#pragma once
#include "CommonDefs.h"


struct Utility
{
    static std::string get_string_from_file( const std::string& file );
    static bool write_string_to_file( const std::string& str, const std::string& file );
    static OptionList extract_options_from_string( const std::string& str );
    static std::ostream& output_options( std::ostream& os, const OptionList& options, const std::string& indent = "" );
};

