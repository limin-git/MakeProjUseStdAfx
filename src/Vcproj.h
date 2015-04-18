#pragma once


using namespace boost::filesystem;


class Vcproj
{
public:

    Vcproj( const path& p, const std::string& configuration_type = "Debug" );

    void save();

private:

    void extract_files();
    void extract_additional_include_directories();
    void extract_VCCLCompilerTool();

    static std::string get_string_from_file( const std::string& file_path );


public:

    path m_path;
    std::string m_str;
    path m_current_path;
    std::vector<path> m_additional_include_directories;
    std::vector<path> m_files;
    std::string m_configuration_type;

    std::string m_VCCLCompilerTool;
};
