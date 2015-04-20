#pragma once
#include "CommonDefs.h"


class FileConfiguration
{
public:

    FileConfiguration( const std::string& str );

    const std::string& generate_fileconfiguration();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
    ToolPtr m_tool;
};
