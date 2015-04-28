#include "StdAfx.h"
#include "VCLinkerToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"
#include "Utility.h"


void VCLinkerToolMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    ToolMaker::initialize( project, configuration_name );

    if ( ! m_tool )
    {
        return;
    }

    make_AdditionalLibraryDirectories();

    if ( m_tool_options->is_changed() )
    {
        m_tool->set_changed();
    }
}


void VCLinkerToolMaker::make_AdditionalLibraryDirectories()
{
    const std::string option_name = "AdditionalLibraryDirectories";
    const std::string& option_value = m_tool_options->get_option_value( option_name );
    bool is_included_ace = false;
    bool is_included_boost = false;
    bool is_included_omniorb = false;

    if ( false == m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->insert_option( option_name, "", OptionListHelper::Before, "GenerateDebugInformation" );
    }
    else
    {
        //cots\omniORB\omniORB_4.1.6\lib\x86_win32
        //cots\ACE\6_0_4\ACE_wrappers\lib
        //cots\boost\boost_1_39_0\stage\lib;
        is_included_ace     = ( option_value.find( "ACE_wrappers" ) != std::string::npos );
        is_included_boost   = ( option_value.find( "boost_1_39_0" ) != std::string::npos );
        is_included_omniorb = ( option_value.find( "omniORB" ) != std::string::npos );
    }

    if ( is_included_ace && is_included_boost && is_included_omniorb )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    path cots_relative_path = Utility::search_parent_relative_path( current_path, "cots" );

    if ( cots_relative_path.empty() )
    {
        boost::smatch m;

        if ( boost::regex_search( option_value, m, Utility::create_regex( "(?x) (^|[,;]) ([^,;]+?) \\b cots \\b" ) ) )
        {
            cots_relative_path = m.str(2);
        }
        else
        {
            std::cout << "\t" << "can not find cots path." << std::endl;
            return;
        }
    }

    const bool is_includes[] = { is_included_ace, is_included_boost, is_included_omniorb };
    const char* cots_lib_pats[] = { "cots\\ACE\\6_0_4\\ACE_wrappers\\lib", "cots\\boost\\boost_1_39_0\\stage\\lib", "cots\\omniORB\\omniORB_4.1.6\\lib\\x86_win32" };
    const size_t cnt = sizeof(cots_lib_pats) / sizeof(char*);
    std::stringstream option_value_strm;

    if ( false == option_value.empty() )
    {
        option_value_strm << option_value;
    }

    for ( size_t i = 0; i < cnt; ++i )
    {
        if ( false == is_includes[i] )
        {
            if ( false == option_value_strm.str().empty() )
            {
                option_value_strm << ";";
            }

            option_value_strm << ( cots_relative_path / cots_lib_pats[i]  ).string();
        }
    }

    m_tool_options->modify_option( option_name, option_value_strm.str() );
}
