#include "StdAfx.h"
#include "RemoveStdAfxMaker.h"
#include "VisualStudioProject.h"
#include "Files.h"


void RemoveStdAfxMaker::initialize( VisualStudioProjectPtr project )
{
    m_project = project;

    if ( m_project )
    {
        m_files = project->m_files;
    }
}


void RemoveStdAfxMaker::make_project( VisualStudioProjectPtr project )
{
    initialize( project );

    if ( m_files )
    {
        remove_stdafx_cpp();
    }
}


void RemoveStdAfxMaker::remove_stdafx_cpp()
{
    std::string& s = m_files->m_str;

    boost::regex e
    (
        "(?xi)"
        "^[\t]+ <File\\n"
        "^[\t]+     RelativePath=\"[^\"]*? StdAfx\\.(cpp|h)\""
        ".+?"
        "^[\t]+ </File>\\n"
    );

    boost::smatch m;

    while ( boost::regex_search( s, m, e ) )
    {
        std::cout << "\t" << "- " << "StdAfx." << m.str(1) << std::endl;
        //std::cout << m.str() << std::endl;
        s.erase( m.position(), m.length() );
        m_files->set_changed();
    }
}
