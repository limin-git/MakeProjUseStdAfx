#include "StdAfx.h"
#include "References.h"


References::References( const std::string& str )
    : m_str( str )
{
}


std::string References::generate_references()
{
    return m_str;
}
