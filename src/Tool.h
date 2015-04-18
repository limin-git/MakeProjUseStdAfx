#pragma once


class Vcproj;
using namespace boost::filesystem;

typedef std::pair<std::string, std::string> Option;
typedef std::list<Option> OptionList;


class Tool
{
public:

    Tool( Vcproj* project, const std::string& str, size_t pos );
    std::string make_tool();
    void save_tool();
    Option& find_option( const std::string& option_name );
    bool is_changed() { return m_changed; }
    std::ostream& output_options( std::ostream& os );

public:

    size_t m_pos;
    bool m_changed;
    const std::string& m_str;
    std::string m_indent_str;
    OptionList m_options;
    Vcproj* m_project;
};
