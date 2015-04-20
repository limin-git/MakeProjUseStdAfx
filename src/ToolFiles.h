#pragma once
#include "CommonDefs.h"


class ToolFiles
{
public:

    ToolFiles( const std::string& str );

    std::string generate_toolfiles();
    bool is_changed();

public:

    std::string m_str;
};
