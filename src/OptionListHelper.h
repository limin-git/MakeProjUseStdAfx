#pragma once
#include "CommonDefs.h"


class OptionListHelper
{
public:

    enum EPosition { Before, After };

public:

    OptionListHelper( OptionList* options );
    Option& get_option( const std::string& option_name );
    const std::string& get_option_value( const std::string& option_name );
    bool is_option_exist( const std::string& option_name );
    void modify_option( const std::string& option_name, const std::string& option_value );
    void insert_option( const std::string& option_name, const std::string& option_value, EPosition pos, const std::string& option );
    void remove_option( const std::string& option_name );
    bool is_changed() { return m_is_changed; }

public:

    OptionList* m_options;
    bool m_is_changed;
};
