#include "StdAfx.h"
#include "VCCLCompilerToolMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"


void VCCLCompilerToolMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;
    m_tool.reset();
    m_tool_options.reset();

    FilesHelper files_helper( m_project, m_configuration_name );
    
    if ( false == files_helper.is_exist( ".cpp" ) )
    {
        return;
    }

    ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

    for ( size_t i = 0; i < configurations.size(); ++i )
    {
        ConfigurationPtr configuration = configurations[i];
        OptionListHelper configuration_options( &configuration->m_options );

        if ( configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
        {
            ToolPtrList& tools = configuration->m_tools;

            for ( size_t i = 0; i < tools.size(); ++i )
            {
                OptionListHelperPtr tool_options( new OptionListHelper( &tools[i]->m_options ) );

                if ( tool_options->get_option_value( "Name" ) == "VCCLCompilerTool" )
                {
                    m_tool = tools[i];
                    m_tool_options = tool_options;
                    return;
                }
            }
        }
    }
}



void VCCLCompilerToolMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    initialize( project, configuration_name );

    if ( !m_tool )
    {
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
        //,"_MBCS", "AFX_RESOURCE_DLL", "_USRDLL", "_WINDLL"
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

    if ( false == m_tool_options->is_option_exist( option_name ) )
    {
        m_tool_options->insert_option( option_name, "", OptionListHelper::Before, "PreprocessorDefinitions" );
    }

    const Option& option = m_tool_options->get_option( option_name );
    //..\..\;..\..\..\cots\boost\boost_1_39_0;..\..\..\cots\ACE\6_0_4\ACE_wrappers;..\..\..\cots\omniORB\omniORB_4.1.6\include
    bool is_included_ace     = ( option.second.find( "ACE_wrappers" ) != std::string::npos );
    bool is_included_boost   = ( option.second.find( "boost_1_39_0" ) != std::string::npos );
    bool is_included_omniorb = ( option.second.find( "omniORB" ) != std::string::npos );

    if ( is_included_ace && is_included_boost && is_included_omniorb )
    {
        return;
    }

    path cots_path = "..";
    path current_path = m_project->m_current_path;
    bool is_path_found = false;

    for ( size_t i = 0; i < 10; ++i )
    {
        if ( boost::filesystem::exists( current_path / cots_path / "cots" ) )
        {
            is_path_found = true;
            break;
        }

        cots_path /= "..";
    }

    if ( false == is_path_found )
    {
        boost::smatch m;

        if ( boost::regex_search( option.second, m, boost::regex( "(?x) (^|[,;]) ([^,;]+?) \\b cots \\b" ) ) )
        {
            cots_path = m.str(2);
        }
        else
        {
            std::cout << "\t" << "can not find cots path." << std::endl;
            return;
        }
    }

    std::stringstream option_value_strm;
    option_value_strm << option.second;

    if ( false == is_included_ace )
    {
        path ace_path = ( cots_path / "cots/ACE/6_0_4/ACE_wrappers" );
        ace_path.make_preferred();
        option_value_strm << ";" << ace_path.string();
    }

    if ( false == is_included_boost )
    {
        path boost_path = ( cots_path / "cots/boost/boost_1_39_0" );
        boost_path.make_preferred();
        option_value_strm << ";" << boost_path.string();
    }

    if ( false == is_included_omniorb )
    {
        path omniorb_path = ( cots_path / "cots/omniORB/omniORB_4.1.6/include" );
        omniorb_path.make_preferred();
        option_value_strm << ";" << omniorb_path.string();
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
