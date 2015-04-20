#pragma once
#include "CommonDefs.h"


class Tool
{
public:

    Tool( const std::string& str );

    std::string generate_tool();
    bool is_changed() { return m_is_changed; }
    void set_changed() { m_is_changed = true; }

public:

    std::string m_str;
    OptionList m_options;
    bool m_is_changed;
};
