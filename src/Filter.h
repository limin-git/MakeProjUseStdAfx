#pragma once
#include "CommonDefs.h"


class Filter
{
public:

    Filter( const std::string& str );

    const std::string& generate_filter();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
    FilterPtrList m_filters;
    FilePtrList m_files;
};
