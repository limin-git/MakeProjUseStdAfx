#pragma once


class Vcproj;
using namespace boost::filesystem;

typedef std::pair<std::string, std::string> Option;
typedef std::list<Option> OptionList;


class Tool
{
public:

    enum ops_type { Before, After };

public:

    virtual void make_tool() = 0;

public:

    Tool( Vcproj* project, const std::string& str, size_t pos );
    std::string generate_tool_string();
    void save_tool();
    Option& find_option( const std::string& option_name );
    bool is_option_exist( const std::string& option_name );
    bool is_changed() { return m_changed; }
    std::ostream& output_options( std::ostream& os );
    void modify_option( const std::string& option_name, const std::string& option_value );
    void insert_option( const std::string& option_name, const std::string& option_value, ops_type pos, const std::string& option );
    void remove_option( const std::string& option_name );

public:

    size_t m_pos;
    bool m_changed;
    std::string m_str;
    std::string m_indent_str;
    OptionList m_options;
    Vcproj* m_project;
};
