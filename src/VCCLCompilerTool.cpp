#include "StdAfx.h"
#include "VCCLCompilerTool.h"
#include "Vcproj.h"


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
    Option& detect = find_option( "AdditionalOptions" );

    if ( detect.first.empty() )
    {
        m_options.push_front( std::make_pair( "AdditionalOptions", "/Zm1000" ) );
        m_changed = true;
        std::cout << "+ AdditionalOptions: /Zm1000" << std::endl;
        return;
    }
    else if ( detect.second.empty() )
    {
        detect.second = "/Zm1000";
        m_changed = true;
        std::cout << "AdditionalOptions: + /Zm1000" << std::endl;
        return;
    }

    Option& option = find_option( "AdditionalOptions" );
    std::string& option_value = option.second;

    if ( option_value.find( "/Zm" ) == std::string::npos )
    {
        option_value += " /Zm1000";
        m_changed = true;
        std::cout << "AdditionalOptions: + /Zm1000" << std::endl;
    }
    else if ( option_value.find( "/Zm1000" ) == std::string::npos )
    {
        option_value = boost::regex_replace( option_value, boost::regex( "/Zm\\d+" ), "/Zm1000" );
        m_changed = true;
        std::cout << "AdditionalOptions: +- /Zm1000" << std::endl;
    }
}


void VCCLCompilerTool::make_AdditionalIncludeDirectories()
{
    Option& option = find_option( "AdditionalIncludeDirectories" );
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
        std::cout << "AdditionalIncludeDirectories: + " << ace_path.string() << std::endl;
    }

    if ( false == is_included_boost )
    {
        path boost_path = ( cots_relative_path / "cots/boost/boost_1_39_0" );
        boost_path.make_preferred();
        option_value += ";";
        option_value += boost_path.string();
        std::cout << "AdditionalIncludeDirectories: + " << boost_path.string() << std::endl;
    }

    if ( false == is_included_omniorb )
    {
        path omniorb_path = ( cots_relative_path / "cots/omniORB/omniORB_4.1.6/include" );
        omniorb_path.make_preferred();
        option_value += ";";
        option_value += omniorb_path.string();
        std::cout << "AdditionalIncludeDirectories: + " << omniorb_path.string() << std::endl;
    }

    m_changed = true;
}


void VCCLCompilerTool::make_PrecompiledHeaderFile()
{
    Option& option = find_option( "PrecompiledHeaderFile" );
    std::string& option_value = option.second;

    // PrecompiledHeaderFile="..\stdafx\$(ConfigurationName)\TA_StdAfx.pch"
    
    if ( option_value.find( "TA_StdAfx.pch" ) != std::string::npos )
    {
        return;
    }

    path current_path = m_project->m_current_path;
    path stdafx_path;
    stdafx_path = stdafx_path / "stdafx" / m_project->m_configuration_type / "TA_StdAfx.pch";

    for ( size_t i = 0; i < 20; ++i )
    {
        if ( boost::filesystem::exists( current_path / stdafx_path ) )
        {
            option_value = stdafx_path.string();
            m_changed = true;
            std::cout << "PrecompiledHeaderFile: +- " << stdafx_path.string() << std::endl;
            return;
        }

        stdafx_path = ".." / stdafx_path;
    }

    std::cout << "can not fine TA_StdAfx.pch." << std::endl;
}


void VCCLCompilerTool::make_UsePrecompiledHeader()
{
    Option& detect = find_option( "UsePrecompiledHeader" );

    if ( detect.first.empty() )
    {
        for ( OptionList::iterator it = m_options.begin(); it != m_options.end(); ++it )
        {
            if ( it->first == "PrecompiledHeaderFile" )
            {
                m_options.insert( it, std::make_pair( "UsePrecompiledHeader", "2" ) );
                m_changed = true;
                std::cout << "+ UsePrecompiledHeader: 2" << std::endl;
                return;
            }
        }

        std::cout << "failed to add PrecompiledHeaderFile." << std::endl;
        return;
    }

    Option& option = find_option( "UsePrecompiledHeader" );
    std::string& option_value = option.second;

    if ( option_value != "2" )
    {
        option_value = "2";
        m_changed = true;
        std::cout << "UsePrecompiledHeader: +- 2" << std::endl;
    }
}
