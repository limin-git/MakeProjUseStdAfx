#include "StdAfx.h"
#include "Solution.h"
#include "Vcproj.h"
#include "Utility.h"


Solution::Solution( const path& p )
    : m_path( p )
{
    std::cout << m_path.string() << std::endl;
    m_current_path = m_path.parent_path();
    m_str = Utility::get_string_from_file( m_path.string() );

    extract_projects();
}


void Solution::extract_projects()
{
    if ( m_str.empty() )
    {
        return;
    }

    // extract additional include directories
    const char* e =
        "(?x)"
        "(?<=\\n) ^ [ \\t]*"                            // starts with spaces
        "\\< Project \\> \\s*"
        "( \\(\"\\{ .*? \\}\"\\) \\s* = \\s* )"         // $1,                                          ("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") =
        "( \" [^\"]+ \", \\s* )"                        // $2, project name :                           "core.alarm.idl",
        "( \" ( [^\"]+ ) \", \\s* )"                    // $3,4, project relative file path             "..\..\transactive\core\alarm\IDL\core.alarm.idl.TA_AlarmAgentCorbaDef.vcproj",
        "( \" [^\"]+ \" \\s* )"                         // $5, other things                             "{7C6902D2-CFA0-4E8B-B4A9-C68815297F8C}"
        " .*? "
        "(?<=\\n) ^ [ \\t]*"                            // starts with spaces
        "\\< EndProject \\>"
        ;
    boost::sregex_iterator it( m_str.begin(), m_str.end(), Utility::create_regex(e) );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        path project_relative_path = it->str(4);

        if ( project_relative_path.extension() == _vcproj )
        {
            path p = boost::filesystem::system_complete( m_current_path / project_relative_path );
            //std::cout << "\t" << p.string() << std::endl;

            if ( is_safe( p ) )
            {
                m_projects.push_back( p );
            }
        }
    }
}


bool Solution::is_safe( const path& p )
{
    static std::set<path> s_black_list;

    if ( s_black_list.empty() )
    {
        s_black_list.insert( "bus.tri_state_entity_tree_ctrl.TA_TriStateEntityTreeCtrl.vcproj" );
        s_black_list.insert( "bus.scada.scada_tree.TA_ScadaTree.vcproj" );
        s_black_list.insert( "bus.generic_gui.ItaUserMessage.TA_ItaUserMessage.vcproj" );
        s_black_list.insert( "bus.sound.sound_client.TA_SoundClient.vcproj" );
    }

    if ( s_black_list.find( p.filename() ) == s_black_list.end() )
    {
        return true;
    }

    std::cout << "\t" << "in black list, ignore: " << p.string() << std::endl;
    return false;
}
