#pragma once
#include "CommonDefs.h"


class Filter
{
public:

    Filter( const std::string& str );

    std::string generate_filter();
    bool is_changed();

public:

    std::string m_str;
    OptionList m_options;
    std::vector<FilterPtr> m_filters;
    std::vector<FilePtr> m_files;
};
