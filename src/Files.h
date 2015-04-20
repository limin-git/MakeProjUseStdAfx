#pragma once
#include "CommonDefs.h"


class Files
{
public:

    Files( const std::string& str );

    std::string generate_files();
    bool is_changed();

public:

    std::string m_str;
    std::vector<FilterPtr> m_filters;
    std::vector<FilePtr> m_files;
};
