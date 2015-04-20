#pragma once
#include "CommonDefs.h"


class Platform
{
public:

    Platform( const std::string& str );

    std::string generate_platform();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
};
