#pragma once
#include "CommonDefs.h"


class Tool
{
public:

    Tool( const std::string& str );

    std::string generate_tool();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
};
