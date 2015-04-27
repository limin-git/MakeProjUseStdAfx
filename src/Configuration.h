#pragma once
#include "CommonDefs.h"


class Configuration
{
public:

    Configuration( const std::string& str );
    std::string generate_configuration();
    bool is_changed();
    void set_changed() { m_is_changed = true; }


public:

    bool m_is_changed;
    std::string m_str;
    OptionList m_options;
    ToolPtrList m_tools;
    std::string m_indent;
};
