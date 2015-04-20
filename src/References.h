#pragma once
#include "CommonDefs.h"


class References
{
public:

    References( const std::string& str );

    const std::string& generate_references();

public:

    std::string m_str;
};
