#pragma once


struct Utility
{
    static std::string get_string_from_file( const std::string& file );
    static bool write_string_to_file( const std::string& str, const std::string& file );
};

