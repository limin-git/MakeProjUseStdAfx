#pragma once
#include "CommonDefs.h"


class File
{
public:

    File( const std::string& str );

    std::string generate_file();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
    FileConfigurationPtrList m_file_configurations;
};
