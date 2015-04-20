#pragma once
#include "CommonDefs.h"


class Configuration
{
public:

    Configuration( const std::string& str );
    std::string generate_configuration();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
    std::vector<ToolPtr> m_tools;
};
