#include "StdAfx.h"
#include "VisualStudioProject.h"
#include "Utility.h"
#include "Platforms.h"
#include "ToolFiles.h"
#include "Configurations.h"
#include "References.h"
#include "Files.h"
#include "Globals.h"
#include "FilesHelper.h"
#include "ProjectHelper.h"


VisualStudioProject::VisualStudioProject( const path& p )
    : m_path( p )
{
    //std::cout << m_path.string() << std::endl;
    m_current_path = m_path.parent_path();
    m_str = Utility::get_string_from_file( m_path );

    boost::smatch m;
    const char* e =
        "(?x)"
        "( <\\?xml [ ]+ version .+? > ) \\n"                            // 1: version
        "( <VisualStudioProject .+? > ) \\n"                            // 2: VisualStudioProject
        "( [ \t]     <Platforms> .*? </Platforms> ) \\n"                // 3: Platforms
        "( [ \t]     <ToolFiles> .*? </ToolFiles> ) \\n"                // 4: ToolFiles
        "( [ \t]     <Configurations> .*? </Configurations> ) \\n"      // 5: Configurations
        "( [ \t]     <References> .*? </References> ) \\n"              // 6: References
        "( [ \t]     <Files> .*? </Files> ) \\n"                        // 7: Files
        "( [ \t]     <Globals> .*? </Globals> ) \\n"                    // 8: Globals
        "  </VisualStudioProject> \\s*"
        ;

    if ( boost::regex_match( m_str, m, Utility::create_regex(e) ) )
    {
        m_version = m.str(1);
        m_options = Utility::extract_options_from_string( m.str(2) );
        m_platforms.reset( new Platforms( m.str(3) ) );
        m_tool_files.reset( new ToolFiles( m.str(4) ) );
        m_configurations.reset( new Configurations( m.str(5) ) );
        m_references.reset( new References( m.str(6) ) );
        m_references.reset( new References( m.str(6) ) );
        m_files.reset( new Files( m.str(7) ) );
        m_globals.reset( new Globals( m.str(8) ) );
    }
    else
    {
        std::cout << "\t" << "failed to parse " << m_path << std::endl;
    }

    // create helpers
    m_project_helper.reset( new ProjectHelper( this ) );
    m_files_helper.reset( new FilesHelper( this ) );
}


std::string VisualStudioProject::generate_visual_studio_project()
{
    if ( false == is_changed() )
    {
        return m_str;
    }

    std::stringstream strm;
    strm
        << m_version << std::endl
        << "<VisualStudioProject\n";
    Utility::output_options( strm, m_options, "\t" );
    strm
        << "\t>\n"
        << m_platforms->generate_platforms() << std::endl
        << m_tool_files->generate_toolfiles() << std::endl
        << m_configurations->generate_configurations() << std::endl
        << m_references->generate_references() << std::endl
        << m_files->generate_files() << std::endl
        << m_globals->generate_globals() << std::endl
        << "</VisualStudioProject>\n"
        ;
    return strm.str();
}


bool VisualStudioProject::is_changed()
{
    bool changed = false;

    if ( m_configurations )
    {
        changed |= m_configurations->is_changed();
    }

    if ( m_files )
    {
        changed |= m_files->is_changed();
    }

    return changed;
}


void VisualStudioProject::save()
{
    if ( false == is_changed() )
    {
        return;
    }

    Utility::write_string_to_file( generate_visual_studio_project(), m_path );
}
