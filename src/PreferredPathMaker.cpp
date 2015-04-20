#include "StdAfx.h"
#include "PreferredPathMaker.h"
#include "VisualStudioProject.h"


PreferredPathMaker::PreferredPathMaker( VisualStudioProjectPtr project )
    : m_project( project )
{
}


void PreferredPathMaker::make_all()
{
    make_preferred_path();
}


void PreferredPathMaker::make_preferred_path()
{
    boost::regex e
    (
        "(?x)"
        "("
        "    OutputDirectory|IntermediateDirectory|InheritedPropertySheets|" // Configuration
        "    AdditionalIncludeDirectories|PrecompiledHeaderFile|AssemblerListingLocation|ObjectFile|ProgramDataBaseFileName|" // Tool - VCCLCompilerTool
        "    OutputFile|" //Tool - VCLibrarianTool|VCLinkerTool|VCBscMakeTool
        "    AdditionalLibraryDirectories" // Tool - VCLinkerTool
        ")"
        "=\" ([^\"]+) \"" // paths
    );

    std::string& str = m_project->m_str;

    bool is_changed = false;
    boost::sregex_iterator it( str.begin(), str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        size_t beg = it->position(2);
        size_t cnt = it->length(2);
        char* p = const_cast<char*>( str.c_str() ) + beg;

        for ( size_t i = 0; i < cnt; ++i, ++p )
        {
            if ( *p == '/' )
            {
                *p = '\\';
                is_changed = true;
            }
        }
    }

    if ( is_changed )
    {
        m_project->set_changed();
    }
}
