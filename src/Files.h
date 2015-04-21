#pragma once
#include "CommonDefs.h"


class Files
{
public:

    Files( const std::string& str );

    const std::string& generate_files();
    bool is_changed();

public:

    std::string m_str;
    FilterPtrList m_filters;
    FilePtrList m_files;
};
