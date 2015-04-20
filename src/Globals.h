#pragma once
#include "CommonDefs.h"


class Globals
{
public:

    Globals( const std::string& str );

    std::string generate_globals();
    bool is_changed();

public:

    std::string m_str;
};
