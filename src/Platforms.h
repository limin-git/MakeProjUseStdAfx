#pragma once
#include "CommonDefs.h"


class Platforms
{
public:

    Platforms( const std::string& str );

    std::string generate_platforms();
    bool is_changed();

public:

    std::string m_str;
    std::vector<PlatformPtr> m_platforms;
};
