#include "StdAfx.h"
#include "Globals.h"


Globals::Globals( const std::string& str )
    : m_str( str )
{
}


const std::string& Globals::generate_globals()
{
    return m_str;
}
