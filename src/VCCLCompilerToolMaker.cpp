#include "StdAfx.h"
#include "VCCLCompilerToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"
#include "Utility.h"
#include "ProjectHelper.h"


void VCCLCompilerToolMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    ToolMaker::initialize( project, configuration_name );

    if ( ! m_tool )
    {
        return;
    }

    const std::string& configuration_type = m_project->m_project_helper->get_configuration_type();

    if ( "1" == configuration_type || "2" == configuration_type )
    {
        make_UsePrecompiledHeader();
        make_AdditionalOptions();
        return;
    }

    make_PreprocessorDefinitions();
    make_AdditionalOptions();
    make_AdditionalIncludeDirectories();
    make_PrecompiledHeaderFile();
    make_UsePrecompiledHeader();
    make_WarningLevel();
    make_BrowseInformation();
    make_MinimalRebuild();

    if ( m_tool_options->is_changed() )
    {
        m_tool->set_changed();
    }
}


void VCCLCompilerToolMaker::make_PreprocessorDefinitions()
{
    const std::string option_name = "PreprocessorDefinitions";
    static const char* preprocessors[] =
    {
        "WIN32", "_DEBUG", "_WINDOWS", "_USE_32BIT_TIME_T", "_AFXDLL", "_AFXEXT", "__WIN32__", "__x86__", "__NT__", "_WIN32_WINNT=0x500", "_CRT_SECURE_NO_DEPRECATE", "__OSVERSION=4"
        //,"_MBCS", "AFX_RESOURCE_DLL", "_USRDLL", "_WINDLL", "_VC80_UPGRADE=0x0600"
    };
    size_t cnt = sizeof(preprocessors) / sizeof(const char*);
    const Option& option = m_tool_options->get_option( option_name );
    std::string option_value = option.second;

    for ( size_t i = 0; i < cnt; ++i )
    {
        if ( option.second.find( preprocessors[i] ) == std::string::npos )
        {
            option_value += ";";
            option_value += preprocessors[i];
        }
    }

    m_tool_options->modify_option( option_name, option_value );
}


void VCCLCompilerToolMaker::make_AdditionalOptions()
{
    const std::string option_name = "AdditionalOptions";
    std::string option_value = "/Zm1000";

    if ( false == m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->insert_option( option_name, option_value, OptionListHelper::After, "Name" );
        return;
    }

    const Option& option = m_tool_options->get_option( option_name );

    if ( option.second.find( option_value ) != std::string::npos )
    {
        return;
    }

    if ( option.second.empty() )
    {
        m_tool_options->modify_option( option_name, option_value );
    }
    else if ( option.second.find( "/Zm" ) == std::string::npos )
    {
        option_value = option.second + " " + option_value;
        m_tool_options->modify_option( option_name, option_value );
    }
    else
    {
        option_value = boost::regex_replace( option.second, boost::regex( "/Zm\\d+" ), option_value );
        m_tool_options->modify_option( option_name, option_value );
    }
}


void VCCLCompilerToolMaker::make_AdditionalIncludeDirectories()
{
    const std::string option_name = "AdditionalIncludeDirectories";
    const std::string& option_value = m_tool_options->get_option_value( option_name );
    bool is_included_ace = false;
    bool is_included_boost = false;
    bool is_included_omniorb = false;

    if ( false == m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->insert_option( option_name, "", OptionListHelper::Before, "PreprocessorDefinitions" );
    }
    else
    {
        // cots\boost\boost_1_39_0
        // cots\ACE\6_0_4\ACE_wrappers
        // cots\omniORB\omniORB_4.1.6\include
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

        if ( boost::regex_search( option_value, m, boost::regex( "(?x) (^|[,;]) ([^,;]+?) \\b cots \\b" ) ) )
        {
            cots_relative_path = m.str(2);
        }
        else
        {
            std::cout << "\t" << "can not find cots path." << std::endl;
            return;
        }
    }

    const bool* is_includes[] = { &is_included_ace, &is_included_boost, &is_included_omniorb };
    const char* cots_lib_pats[] = { "cots\\ACE\\6_0_4\\ACE_wrappers", "cots\\boost\\boost_1_39_0", "cots\\omniORB\\omniORB_4.1.6\\include" };
    const size_t cnt = sizeof(cots_lib_pats) / sizeof(char*);
    std::stringstream option_value_strm;

    if ( false == option_value.empty() )
    {
        option_value_strm << option_value;
    }

    for ( size_t i = 0; i < cnt; ++i )
    {
        if ( false == *is_includes[i] )
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


void VCCLCompilerToolMaker::make_PrecompiledHeaderFile()
{
    const std::string option_name = "PrecompiledHeaderFile";
    const std::string stdafx_pch = "TA_StdAfx.pch";

    if ( m_tool_options->is_option_exist( option_name ) )
    {
        const Option& option = m_tool_options->get_option( option_name );

        if ( option.second.find( stdafx_pch ) != std::string::npos )
        {
            return;
        }
    }

    path current_path = m_project->m_current_path;

    const char* short_paths[] = { "stdafx", "core\\stdafx", "transactive\\core\\stdafx", "code\\transactive\\core\\stdafx" };
    size_t cnt = sizeof(short_paths) / sizeof(const char*);

    for ( size_t i = 0; i < cnt; ++i  )
    {
        path stdafx_relative_path = "..";

        for ( size_t j = 0; j < 10; ++j )
        {
            if ( boost::filesystem::exists( current_path / stdafx_relative_path / short_paths[i] / m_configuration_name / stdafx_pch ) )
            {
                std::string option_value = ( stdafx_relative_path / short_paths[i] / "$(ConfigurationName)" / stdafx_pch ).string();

                if ( m_tool_options->is_option_exist( option_name ) )
                {
                    m_tool_options->modify_option( option_name, option_value );
                }
                else
                {
                    m_tool_options->insert_option( option_name, option_value, OptionListHelper::Before, "AssemblerListingLocation" );
                }

                return;
            }

            stdafx_relative_path /= "..";
        }
    }

    std::cout << "\t" << "can not find TA_StdAfx.pch." << std::endl;
}


void VCCLCompilerToolMaker::make_UsePrecompiledHeader()
{
    const std::string option_name = "UsePrecompiledHeader";
    const std::string option_value = "2";

    if ( m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->modify_option( option_name,option_value );
    }
    else
    {
        m_tool_options->insert_option( option_name, option_value, OptionListHelper::Before, "PrecompiledHeaderFile" );
    }
}


void VCCLCompilerToolMaker::make_WarningLevel()
{
    const std::string option_name = "WarningLevel";
    const std::string option_value = "3";

    if ( m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->modify_option( option_name,option_value );
    }
    else
    {
        m_tool_options->insert_option( option_name, option_value, OptionListHelper::Before, "SuppressStartupBanner" );
    }
}


void VCCLCompilerToolMaker::make_BrowseInformation()
{
    const std::string option_name = "BrowseInformation";
    m_tool_options->remove_option( option_name );
}


void VCCLCompilerToolMaker::make_MinimalRebuild()
{
    const std::string option_name = "MinimalRebuild";
    const std::string option_value = "true";

    if ( m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->modify_option( option_name, option_value );
    }
    else
    {
        m_tool_options->insert_option( option_name, option_value, OptionListHelper::After, "PreprocessorDefinitions" );
    }
}
