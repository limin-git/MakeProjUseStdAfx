#include "StdAfx.h"
#include "Vcproj.h"
#include "VCCLCompilerTool.h"
#include "VCPreBuildEventTool.h"


Vcproj::Vcproj( const path& p, const std::string& configuration_type )
    : m_path( p ),
      m_configuration_type( configuration_type )
{
    std::cout << m_path.string() << std::endl;
    m_current_path = m_path.parent_path();
    m_str = get_string_from_file( m_path.string() );
    //std::cout << m_current_path.string() << std::endl;

    //extract_files();
    //extract_additional_include_directories();
    //extract_VCCLCompilerTool();
    //extract_VCPreBuildEventTool();

    make_preferred_path();
}


std::string Vcproj::get_string_from_file( const std::string& file_path )
{
    std::ifstream ifs( file_path.c_str() );

    if ( ifs )
    {
        return std::string( std::istreambuf_iterator< char >( ifs ), ( std::istreambuf_iterator< char >() ) );
    }

    return "";
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

    static const boost::regex file_configuration_regex
    (
        "(?x)"
        "<FileConfiguration \\s+"
        "Name= \" ( .+? )  \" \\s+"                 //$1, Name
        "ExcludedFromBuild= \" ( .+? ) \" \\s+ "    //$2, ExcludedFromBuild
        ">"
    );

    boost::sregex_iterator it( m_str.begin(), m_str.end(), file_regex );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        path source_file_relative_path = it->str(1);
        std::string configurations = it->str(2);

        bool is_excluded = false;

        if ( false == configurations.empty() )
        {
            boost::sregex_iterator fc_it( configurations.begin(), configurations.end(), file_configuration_regex );
            boost::sregex_iterator end;

            for ( ; fc_it != end; ++fc_it )
            {
                std::string file_configuration_name = fc_it->str(1);
                std::string excluded_form_build = fc_it->str(2);

                std::stringstream match_strm;
                match_strm << m_configuration_type << "|Win32";

                if ( file_configuration_name == match_strm.str() )
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

            if ( boost::ends_with( p.string(), "cpp" ) )
            {
                //std::cout << p.string() << std::endl;
                m_files.push_back( p );
            }
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
        << "    Name=\"" << m_configuration_type << "\\|Win32\""
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
        //std::cout << dirs << std::endl;

        typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep( ";," );
        tokenizer tokens( dirs, sep );

        for ( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
        {
            path p(*it);
            //std::cout << p.string() << std::endl;
            p.make_preferred();
            m_additional_include_directories.push_back( p );
        }
    }
}


void Vcproj::extract_VCCLCompilerTool()
{
    if ( m_str.empty() )
    {
        return;
    }

    std::stringstream configuration_regex_strm;
    configuration_regex_strm
        << "(?x)"
        << "<Configuration \\s+"
        << "    Name=\"" << m_configuration_type << "\\|Win32\""
        << "    .+?"
        << "    ( <Tool \\s+ Name=\"VCCLCompilerTool\" .+? /> )"
        << "    .+?"
        << "</Configuration>"
        ;
    static const boost::regex e( configuration_regex_strm.str() );
    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        m_VCCLCompilerTool = m.str(1);
        //std::cout << m.position(1) << std::endl;
        //std::cout << m_str.substr( m.position(1), m.length(1) ) << std::endl;
        //std::string::difference_type pos = m.position(0);
        //<< m.position(0) << std::endl;
        //std::cout << m_str.substr( 0, m.length(1) ) << std::endl;
        //std::cout << m_VCCLCompilerTool << std::endl;
        VCCLCompilerTool tool( this, m_str, m_VCCLCompilerTool, m.position(1) );
        //std::cout << tool.make_tool() << std::endl;
        tool.make_PreprocessorDefinitions();
        tool.make_AdditionalOptions();
        tool.make_AdditionalIncludeDirectories();
        tool.make_PrecompiledHeaderFile();
        tool.make_UsePrecompiledHeader();
        tool.save_tool();

        if ( tool.m_is_changed )
        {
            save();
        }
    }
}


void Vcproj::extract_VCPreBuildEventTool()
{
    if ( m_str.empty() )
    {
        return;
    }

    std::stringstream configuration_regex_strm;
    configuration_regex_strm
        << "(?x)"
        << "<Configuration \\s+"
        << "    Name=\"" << m_configuration_type << "\\|Win32\""
        << "    .+?"
        << "    ( <Tool \\s+ Name=\"VCPreBuildEventTool\" .+? /> )"
        << "    .+?"
        << "</Configuration>"
        ;
    static const boost::regex e( configuration_regex_strm.str() );
    boost::smatch m;

    if ( boost::regex_search( m_str, m, e ) )
    {
        m_VCPreBuildEventTool = m.str(1);
        //std::cout << m.position(1) << std::endl;
        //std::cout << m_str.substr( m.position(1), m.length(1) ) << std::endl;
        //std::string::difference_type pos = m.position(0);
        //<< m.position(0) << std::endl;
        //std::cout << m_str.substr( 0, m.length(1) ) << std::endl;
        std::cout << m_VCPreBuildEventTool << std::endl;
        VCPreBuildEventTool tool( this, m_str, m_VCPreBuildEventTool, m.position(1) );
        //std::cout << tool.make_tool() << std::endl;
        tool.make_CommandLine();
        tool.save_tool();

        if ( tool.m_is_changed )
        {
            save();
        }
    }
}


void Vcproj::save()
{
    std::ofstream ofs( m_path.string().c_str() );

    if ( !ofs )
    {
        std::cout << "cannot open file " << m_path.string() << std::endl;
        return;
    }

    ofs << m_str;
}


void Vcproj::make_preferred_path()
{
    boost::regex e
    (
        "(?x)"
        "(OutputDirectory|IntermediateDirectory|InheritedPropertySheets|AdditionalIncludeDirectories|PrecompiledHeaderFile|AssemblerListingLocation|ObjectFile|ProgramDataBaseFileName|OutputFile)"
        "=\" ([^\"]+) \""
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
}
