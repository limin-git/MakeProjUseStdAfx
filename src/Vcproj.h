#pragma once


using namespace boost::filesystem;


class Vcproj
{
public:

    Vcproj( const path& p, const std::string& configuration_type = "Debug" );

    void make_project();

private:

    void extract_files();
    void extract_additional_include_directories();
    void make_VCCLCompilerTool();
    void make_VCPreBuildEventTool();

    void make_preferred_path();

    void add_include_StdAfx_for_cpps();

    void save();


public:

    path m_path;
    std::string m_str;
    path m_current_path;
    std::vector<path> m_additional_include_directories;
    std::vector<path> m_files;
    std::string m_configuration_type;

    std::string m_VCCLCompilerTool;
    std::string m_VCPreBuildEventTool;
};
