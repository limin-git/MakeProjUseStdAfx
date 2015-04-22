#pragma once
#include "CommonDefs.h"


class Files
{
public:

    Files( const std::string& str );

    const std::string& generate_files();

    bool is_changed() { return m_is_changed; }
    void set_changed() { m_is_changed = true; }

public:

    bool m_is_changed;
    std::string m_str;
    FilterPtrList m_filters;
    FilePtrList m_files;
};
