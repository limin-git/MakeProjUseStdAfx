#pragma once
#include "CommonDefs.h"


class Configurations
{
public:

    Configurations( const std::string& str );

    std::string generate_configurations();
    bool is_changed();

public:

    std::string m_str;
    std::vector<ConfigurationPtr> m_configurations;
};
