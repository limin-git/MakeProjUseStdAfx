#include "StdAfx.h"
#include "ToolFiles.h"


ToolFiles::ToolFiles( const std::string& str )
    : m_str( str )
{
}


std::string ToolFiles::generate_toolfiles()
{
    return m_str;
}
