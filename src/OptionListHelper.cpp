#include "StdAfx.h"
#include "OptionListHelper.h"


OptionListHelper::OptionListHelper( OptionList* options )
    : m_options( options ),
      m_is_changed( false )
{
}


Option& OptionListHelper::get_option( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options->begin(); it != m_options->end(); ++it )
    {
        if ( it->first == option_name )
        {
            return *it;
        }
    }

    static Option empty;
    return empty;
}


bool OptionListHelper::is_option_exist( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options->begin(); it != m_options->end(); ++it )
    {
        if ( it->first == option_name )
        {
            return true;
        }
    }

    return false;
}


void OptionListHelper::modify_option( const std::string& option_name, const std::string& option_value )
{
    if ( false == is_option_exist( option_name ) )
    {
        std::cout << "\t" << option_name << " does not exist, cannot modify." << std::endl;
        return;
    }

    Option& option = get_option( option_name );

    if ( option.second != option_value )
    {
        option.second = option_value;
        m_is_changed = true;
        std::cout << "\t" << option_name << ": +- " << option_value << std::endl;
    }
}


void OptionListHelper::insert_option( const std::string& option_name, const std::string& option_value, EPosition pos, const std::string& option )
{
    if ( true == is_option_exist( option_name ) )
    {
        std::cout << "\t" << option_name << " already exist, cannot insert new." << std::endl;
        return;
    }

    OptionList::iterator insert_it = m_options->end();

    for ( OptionList::iterator it = m_options->begin(); it != m_options->end(); ++it )
    {
        if ( it->first == option )
        {
            if ( After == pos )
            {
                ++it;
            }

            insert_it = it;
            break;
        }
    }

    m_options->insert( insert_it, std::make_pair(option_name, option_value) );
    m_is_changed = true;
    std::cout << "\t" << "+ " << option_name << ": " << option_value << std::endl;
}


void OptionListHelper::remove_option( const std::string& option_name )
{
    for ( OptionList::iterator it = m_options->begin(); it != m_options->end(); ++it )
    {
        if ( it->first == option_name )
        {
            std::cout << "\t" << "- " << option_name << ": " << it->second << std::endl;
            m_options->erase( it );
            m_is_changed = true;
            return;
        }
    }
}
