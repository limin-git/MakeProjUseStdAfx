#include "StdAfx.h"
#include "Solution.h"
#include "VisualStudioProject.h"
#include "VCCLCompilerToolMaker.h"
#include "VCPreBuildEventToolMaker.h"
#include "IncludeStdAfxMaker.h"
#include "PreferredPathMaker.h"
#include "RemoveStdAfxMaker.h"
#include "VCLinkerToolMaker.h"
#include "ProjectHelper.h"
#include "FilesHelper.h"
#include "GenerateStdAfxMaker.h"


void main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        std::cout
            << "Usage: \n"
            << "\t" << path( argv[0] ).filename().string() << " < .sln | .vcproj > " << std::endl;
        return;
    }
    
    path p = argv[1];

    if ( p.extension() != ".sln" && p.extension() != ".vcproj" )
    {
        std::cout << "only support .solution, .vcproj." << std::endl;
        return;
    }

    VisualStudioProjectPtrList projects;

    if ( p.extension() == ".sln" )
    {
        Solution solution( p );
        const std::vector<path>&  project_paths = solution.get_project_paths();

        for ( size_t i = 0; i < project_paths.size(); ++i )
        {
            VisualStudioProjectPtr project( new VisualStudioProject( project_paths[i] ) );
            projects.push_back( project );
        }
    }
    else
    {
        VisualStudioProjectPtr project( new VisualStudioProject( p ) );
        projects.push_back( project );
    }

    VCCLCompilerToolMaker       compile;
    VCPreBuildEventToolMaker    pre_build;
    IncludeStdAfxMaker          add_include_stdafx;
    PreferredPathMaker          preferred_path;
    //RemoveStdAfxMaker         remove_stdafx;
    VCLinkerToolMaker           link;
    GenerateStdAfxMaker         generate_stdafx;

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        const std::string& configuration_type = projects[i]->m_project_helper->get_configuration_type();
        FilesHelperPtr files_helper = projects[i]->m_files_helper;

        if ( ( configuration_type != "1" &&  configuration_type != "4" ) || // 1: Application (.exe), 4: Static Library (.lib)
             ( files_helper->has_file( "StdAfx.h" ) || files_helper->has_file( "StdAfx.cpp" ) )
           )
        {
            continue;
        }

        std::cout << projects[i]->m_path.string() << std::endl;

        preferred_path.make_project( projects[i] );
        add_include_stdafx.make_project( projects[i] );

        if ( "4" == configuration_type )
        {
            compile.make_project( projects[i] );
            pre_build.make_project( projects[i] );
        }

        if ( "1" == configuration_type )
        {
            link.make_project( projects[i] );
            generate_stdafx.make_project( projects[i] );
        }
    }

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        projects[i]->save();
    }
}
