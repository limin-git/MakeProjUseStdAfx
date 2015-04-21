#include "StdAfx.h"
#include "IncludeStdAfxMaker.h"
#include "VisualStudioProject.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Tool.h"
#include "Files.h"
#include "Filter.h"
#include "File.h"
#include "FileConfiguration.h"
#include "OptionListHelper.h"
#include "FilesHelper.h"
#include "Utility.h"


void IncludeStdAfxMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;
    m_files_helper.reset( new FilesHelper( project ) );
}


void IncludeStdAfxMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    initialize( project, configuration_name );

    add_include_StdAfx_h();
}


void IncludeStdAfxMaker::add_include_StdAfx_h()
{
    const char* include_stdafx_h = "#include \"StdAfx.h\"\n";

    if ( false == m_files_helper->is_exist() )
    {
        return;
    }

    std::vector<path> paths = m_files_helper->get_paths_by_extension( ".cpp" );

    for ( size_t i = 0; i < paths.size(); ++i )
    {
        path& p = paths[i];
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
