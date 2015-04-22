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

    if ( p.extension() == ".solution" )
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

    VCCLCompilerToolMaker m1;
    VCPreBuildEventToolMaker m2;
    IncludeStdAfxMaker m3;
    PreferredPathMaker m4;
    //RemoveStdAfxMaker m5;
    VCLinkerToolMaker m6;

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        std::cout << projects[i]->m_path.string() << std::endl;

        if ( projects[i]->m_project_helper->get_configuration_type() != "4" ||
             projects[i]->m_files_helper->has_file( "StdAfx.h" ) )
        {
            continue;
        }

        m1.make_project( projects[i] );
        m2.make_project( projects[i] );
        m3.make_project( projects[i] );
        m4.make_project( projects[i] );
        //m5.make_project( projects[i] );
        m6.make_project( projects[i] );
    }

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        projects[i]->save();
    }
}
