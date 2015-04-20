#include "StdAfx.h"
#include "VisualStudioProject.h"
#include "Utility.h"
#include "Platforms.h"
#include "ToolFiles.h"
#include "Configurations.h"
#include "References.h"
#include "Files.h"
#include "Globals.h"


VisualStudioProject::VisualStudioProject( const std::string& str )
    : m_str( str )
{
    static const boost::regex e
    (
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
    );

    boost::smatch m;

    if ( boost::regex_match( m_str, m, e ) )
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
        std::cout << "\t" << "failed to parse .vcproj." << std::endl;
    }
}


std::string VisualStudioProject::generate_visual_studio_project()
{
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
