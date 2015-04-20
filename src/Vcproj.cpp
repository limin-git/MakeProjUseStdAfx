#include "StdAfx.h"
#include "Vcproj.h"
#include "Utility.h"
#include "VCCLCompilerTool.h"
#include "VCPreBuildEventTool.h"
#include "VisualStudioProject.h"


Vcproj::Vcproj( const path& p, const std::string& configuration_name )
    : m_path( p ),
      m_configuration_name( configuration_name )
{
    std::cout << m_path.string() << std::endl;
    m_current_path = m_path.parent_path();
    m_str = Utility::get_string_from_file( m_path.string() );


    m_visual_studio_project.reset( new VisualStudioProject( m_str ) );

    //extract_files();
    //extract_additional_include_directories();
}


void Vcproj::make_project()
{
    // project
    make_preferred_path();
    make_VCCLCompilerTool();
    make_VCPreBuildEventTool();

    // files
    add_include_StdAfx_for_cpps();
}


void Vcproj::extract_files()
{
    if ( m_str.empty() )
    {
        return;
    }

    // extract files
    static const boost::regex file_regex
    (
        "(?x)"
        "<File \\s+"
        "    RelativePath=\" ( [^\"]+ ) \" \\s+ > "   //$1, file relative path
        "    ( .*? ) \\s+ "                           //$2, configurations
        "</File>"
    );
    boost::sregex_iterator it( m_str.begin(), m_str.end(), file_regex );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        path source_file_relative_path = it->str(1);
        std::string configurations = it->str(2);
        bool is_excluded = false;

        if ( ! boost::ends_with( it->str(1), "cpp" )  )
        {
            continue;
        }

        if ( false == configurations.empty() )
        {
            static const boost::regex file_configuration_regex
            (
                "(?x)"
                "<FileConfiguration \\s+"
                "    Name= \" ( .+? )  \" \\s+"                 //$1, Name
                "    ExcludedFromBuild= \" ( .+? ) \" \\s+ "    //$2, ExcludedFromBuild
                ">"
            );
            boost::sregex_iterator it( configurations.begin(), configurations.end(), file_configuration_regex );
            boost::sregex_iterator end;

            for ( ; it != end; ++it )
            {
                std::string file_configuration_name = it->str(1);
                std::string excluded_form_build = it->str(2);

                if ( file_configuration_name.find( m_configuration_name ) != std::string::npos )
                {
                    if ( "true" == excluded_form_build )
                    {
                        is_excluded = true;
                    }

                    break;
                }
            }
        }

        if ( false == is_excluded )
        {
            path p = boost::filesystem::system_complete( m_current_path / source_file_relative_path  );
            m_files.push_back( p );
            //std::cout << "\t" << p.string() << std::endl;
        }
    }
}


void Vcproj::extract_additional_include_directories()
{
    if ( m_str.empty() )
    {
        return;
    }

    std::stringstream configuration_regex_strm;
    configuration_regex_strm
        << "(?x)"
        << "<Configuration \\s+"
        << "    Name=\"" << m_configuration_name << "\\|Win32\""
        << "    .+?"
        << "    AdditionalIncludeDirectories=\" ([^\"]*) \""
        << "    .+?"
        << "</Configuration>"
        ;
    static const boost::regex e( configuration_regex_strm.str() );
    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        std::string dirs = m.str(1);
        //std::cout << "\t" << dirs << std::endl;

        typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep( ";," );
        tokenizer tokens( dirs, sep );

        for ( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
        {
            path p(*it);
            //std::cout << "\t" << p.string() << std::endl;
            p.make_preferred();
            m_additional_include_directories.push_back( p );
        }
    }
}


void Vcproj::make_VCCLCompilerTool()
{
    if ( m_str.empty() )
    {
        return;
    }

    if ( false == is_files_exist() )
    {
        return;
    }

    std::stringstream configuration_regex_strm;
    configuration_regex_strm
        << "(?x)"
        << "<Configuration \\s+"
        << "    Name=\"" << m_configuration_name << "\\|Win32\""
        << "    .+?"
        << "    ConfigurationType=\" (\\d+) \""                     // ConfigurationType
        << "    .+?"
        << "    ( <Tool \\s+ Name=\"VCCLCompilerTool\" .+? /> )"    // Tool
        << "    .+?"
        << "</Configuration>"
        ;
    static const boost::regex e( configuration_regex_strm.str() );
    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        std::string configuration_type = m.str(1);

        // 0: MakeFile
        // 1: Application (.exe)
        // 2: Dynamic Library (.dll)
        // 4: Static Library (.lib)
        //10: Utility

        if ( "1" == configuration_type || "2" == configuration_type || "4" == configuration_type )
        {
            VCCLCompilerTool tool( this, m.str(2), m.position(2) );

            tool.make_tool();

            if ( tool.is_changed() )
            {
                save();
            }
        }
    }
}


void Vcproj::make_VCPreBuildEventTool()
{
    if ( m_str.empty() )
    {
        return;
    }

    if ( false == is_files_exist() )
    {
        return;
    }

    std::stringstream configuration_regex_strm;
    configuration_regex_strm
        << "(?x)"
        << "<Configuration \\s+"
        << "    Name=\"" << m_configuration_name << "\\|Win32\""
        << "    .+?"
        << "    ConfigurationType=\" (\\d+) \""                     // ConfigurationType
        << "    .+?"
        << "    ( <Tool \\s+ Name=\"VCPreBuildEventTool\" .+? /> )" // Tool
        << "    .+?"
        << "</Configuration>"
        ;
    static const boost::regex e( configuration_regex_strm.str() );
    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        std::string configuration_type = m.str(1);

        // 0: MakeFile
        // 1: Application (.exe)
        // 2: Dynamic Library (.dll)
        // 4: Static Library (.lib)
        //10: Utility

        if ( "1" == configuration_type || "2" == configuration_type || "4" == configuration_type )
        {
            VCPreBuildEventTool tool( this, m.str(2), m.position(2) );

            tool.make_tool();

            if ( tool.is_changed() )
            {
                save();
            }
        }
    }
}


void Vcproj::make_preferred_path()
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

    bool is_changed = false;
    boost::sregex_iterator it( m_str.begin(), m_str.end(), e );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        size_t beg = it->position(2);
        size_t cnt = it->length(2);
        char* p = const_cast<char*>( m_str.c_str() ) + beg;

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
        save();
    }
}


void Vcproj::add_include_StdAfx_for_cpps()
{
    const char* include_stdafx_h = "#include \"StdAfx.h\"\n";

    if ( false == is_files_exist() )
    {
        return;
    }

    for ( size_t i = 0; i < m_files.size(); ++i )
    {
        path& p = m_files[i];
        std::string str = Utility::get_string_from_file( p.string() );

        // already included
        if ( boost::regex_search( str, boost::regex( "(?xi) \\#include \\s+ \"StdAfx\\.h\"" ) ) )
        {
            continue;
        }

        size_t pos = std::string::npos;

        if ( boost::regex_match( str, boost::regex( "\\s*" ) ) )
        {
            pos = 0;
        }
        else
        {
            boost::smatch m;

            if ( ! boost::regex_search( str, m, boost::regex( "(?x) ^ [ \t]* (\\#include|\\#if|\\#define|namespace) " ) ) )
            {
                std::cout << "\t" << "cannot add include StdAfx for this file: " << p.string() << std::endl;
                continue;
            }

            pos = m.position();
        }

        if ( pos != std::string::npos )
        {
            str.insert( pos, include_stdafx_h );
            Utility::write_string_to_file( str, p.string() );
            std::cout << "\t" << "+ #include \"StdAfx.h\" - " << p.string() << std::endl;
        }
    }
}


void Vcproj::save()
{
    Utility::write_string_to_file( m_str, m_path.string() );
}


bool Vcproj::is_files_exist()
{
    for ( size_t i = 0; i < m_files.size(); ++i )
    {
        if ( false == boost::filesystem::exists( m_files[i] ) )
        {
            return false;
        }
    }

    return true;
}
