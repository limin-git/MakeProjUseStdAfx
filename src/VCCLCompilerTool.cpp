#include "StdAfx.h"
#include "VCCLCompilerTool.h"
#include "Vcproj.h"


void VCCLCompilerTool::make_tool()
{
   make_PreprocessorDefinitions();
   make_AdditionalOptions();
   make_AdditionalIncludeDirectories();
   make_PrecompiledHeaderFile();
   make_UsePrecompiledHeader();
   make_WarningLevel();
   make_BrowseInformation();
   make_MinimalRebuild();
   save_tool();
}


void VCCLCompilerTool::make_PreprocessorDefinitions()
{
    static const char* preprocessors[] =
    {
        "WIN32", "_DEBUG", "_WINDOWS", "_USE_32BIT_TIME_T", "_AFXDLL", "_AFXEXT", "__WIN32__", "__x86__", "__NT__", "_WIN32_WINNT=0x500", "_CRT_SECURE_NO_DEPRECATE", "__OSVERSION=4"
    };

    size_t cnt = sizeof(preprocessors) / sizeof(const char*);

    Option& option = find_option( "PreprocessorDefinitions" );
    std::string& option_value = option.second;

    for ( size_t i = 0; i < cnt; ++i )
    {
        if ( option_value.find( preprocessors[i] ) == std::string::npos )
        {
            option_value += ";";
            option_value += preprocessors[i];
            std::cout << "PreprocessorDefinitions: + " << preprocessors[i] << std::endl;
            m_changed = true;
        }
    }
}


void VCCLCompilerTool::make_AdditionalOptions()
{
    const std::string option_name = "AdditionalOptions";
    const std::string option_value = "/Zm1000";

    if ( false == Tool::is_option_exist( option_name ) )
    {
        Tool::insert_option( option_name, option_value, Tool::AFTER, "Name" );
        return;
    }

    Option& option = Tool::find_option( option_name );

    if ( option.second.empty() )
    {
        Tool::modify_option( option_name, option_value );
    }
    else if ( option.second.find( "/Zm" ) == std::string::npos )
    {
        option.second += " /Zm1000";
        m_changed = true;
        std::cout << option_name << ": + " << option_value << std::endl;
    }
    else if ( option.second.find( option_value ) == std::string::npos )
    {
        option.second = boost::regex_replace( option.second, boost::regex( "/Zm\\d+" ), option_value );
        m_changed = true;
        std::cout << option_name << ": +- " << option_value << std::endl;
    }
}


void VCCLCompilerTool::make_AdditionalIncludeDirectories()
{
    const std::string option_name = "AdditionalIncludeDirectories";

    if ( false == Tool::is_option_exist( option_name ) )
    {
        Tool::insert_option( option_name, "", Tool::BEFORE, "PreprocessorDefinitions" );
    }

    Option& option = find_option( option_name );
    std::string& option_value = option.second;

    //..\..\;..\..\..\cots\boost\boost_1_39_0;..\..\..\cots\ACE\6_0_4\ACE_wrappers;..\..\..\cots\omniORB\omniORB_4.1.6\include

    bool is_included_ace     = ( option_value.find( "ACE_wrappers" ) != std::string::npos );
    bool is_included_boost   = ( option_value.find( "boost_1_39_0" ) != std::string::npos );
    bool is_included_omniorb = ( option_value.find( "omniORB" ) != std::string::npos );

    if ( is_included_ace && is_included_boost && is_included_omniorb )
    {
        return;
    }

    path cots_relative_path;
    path current_path = m_project->m_current_path;
    bool is_found_cots_path = false;

    for ( size_t i = 0; i < 20; ++i )
    {
        if ( boost::filesystem::exists( current_path / cots_relative_path / "cots" ) )
        {
            is_found_cots_path = true;
            break;
        }

        cots_relative_path /= "..";
    }

    if ( false == is_found_cots_path )
    {
        boost::smatch m;

        if ( boost::regex_search( option_value, m, boost::regex( "(?x) (^|[,;]) ([^,;]+?) \\b cots \\b" ) ) )
        {
            cots_relative_path = m.str(2);
        }
        else
        {
            std::cout << "can not determin cots path." << std::endl;
            return;
        }
    }

    if ( false == is_included_ace )
    {
        path ace_path = ( cots_relative_path / "cots/ACE/6_0_4/ACE_wrappers" );
        ace_path.make_preferred();
        option_value += ";";
        option_value += ace_path.string();
        m_changed = true;
        std::cout << "AdditionalIncludeDirectories: + " << ace_path.string() << std::endl;
    }

    if ( false == is_included_boost )
    {
        path boost_path = ( cots_relative_path / "cots/boost/boost_1_39_0" );
        boost_path.make_preferred();
        option_value += ";";
        option_value += boost_path.string();
        m_changed = true;
        std::cout << "AdditionalIncludeDirectories: + " << boost_path.string() << std::endl;
    }

    if ( false == is_included_omniorb )
    {
        path omniorb_path = ( cots_relative_path / "cots/omniORB/omniORB_4.1.6/include" );
        omniorb_path.make_preferred();
        option_value += ";";
        option_value += omniorb_path.string();
        m_changed = true;
        std::cout << "AdditionalIncludeDirectories: + " << omniorb_path.string() << std::endl;
    }
}


void VCCLCompilerTool::make_PrecompiledHeaderFile()
{
    const std::string option_name = "PrecompiledHeaderFile";

    if ( Tool::is_option_exist( option_name ) )
    {
        Option& option = find_option( option_name );

        if ( option.second.find( "TA_StdAfx.pch" ) != std::string::npos )
        {
            return;
        }
    }

    path current_path = m_project->m_current_path;
    path stdafx_path;
    stdafx_path = stdafx_path / "stdafx" / m_project->m_configuration_name / "TA_StdAfx.pch";

    for ( size_t i = 0; i < 10; ++i )
    {
        if ( boost::filesystem::exists( current_path / stdafx_path ) )
        {
            std::string option_value = stdafx_path.string();
            boost::replace_first( option_value, m_project->m_configuration_name, "$(ConfigurationName)" );

            if ( Tool::is_option_exist( option_name ) )
            {
                Tool::modify_option( option_name, option_value );
            }
            else
            {
                Tool::insert_option( option_name, option_value, Tool::BEFORE, "AssemblerListingLocation" );
            }

            return;
        }

        stdafx_path = ".." / stdafx_path;
    }

    std::cout << "can not find TA_StdAfx.pch." << std::endl;
}


void VCCLCompilerTool::make_UsePrecompiledHeader()
{
    const std::string option_name = "UsePrecompiledHeader";
    const std::string option_value = "2";

    if ( false == Tool::is_option_exist( option_name ) )
    {
        Tool::insert_option( option_name, option_value, Tool::BEFORE, "PrecompiledHeaderFile" );
    }
    else
    {
        modify_option( option_name,option_value );
    }
}


void VCCLCompilerTool::make_WarningLevel()
{
    const std::string option_name = "WarningLevel";
    const std::string option_value = "3";

    if ( false == Tool::is_option_exist( option_name ) )
    {
        Tool::insert_option( option_name, option_value, Tool::BEFORE, "SuppressStartupBanner" );
    }
    else
    {
        modify_option( option_name,option_value );
    }
}


void VCCLCompilerTool::make_BrowseInformation()
{
    const std::string option_name = "BrowseInformation";
    Tool::remove_option( option_name );
}


void VCCLCompilerTool::make_MinimalRebuild()
{
    const std::string option_name = "MinimalRebuild";
    const std::string option_value = "true";

    if ( false == Tool::is_option_exist( option_name ) )
    {
        Tool::insert_option( option_name, option_value, Tool::AFTER, "PreprocessorDefinitions" );
    }
    else
    {
        Tool::modify_option( option_name, option_value );
    }
}
