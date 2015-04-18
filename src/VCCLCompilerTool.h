#pragma once


using namespace boost::filesystem;
typedef std::pair<std::string, std::string> OptionType;
typedef std::list<OptionType> OptionsType;


class Vcproj;


class VCCLCompilerTool
{
public:

    VCCLCompilerTool( Vcproj* project, std::string& proj_str, const std::string& str, size_t pos );
    static std::ostream& output( std::ostream& os, const OptionsType& options, const std::string& indent = "" );
    std::string make_tool();
    void save_tool();
    void make_PreprocessorDefinitions();
    void make_AdditionalOptions();
    void make_AdditionalIncludeDirectories();
    void make_PrecompiledHeaderFile();
    void make_UsePrecompiledHeader();
    OptionType& find_option( const std::string& option_name );
    bool is_changed() { return m_is_changed; }
    
public:

    std::string& m_project_string;
    const std::string& m_string;
    size_t m_pos;
    OptionsType m_options;
    std::string m_indent_string;
    bool m_is_changed;
    const OptionType m_empty_option;
    Vcproj* m_project;
};
