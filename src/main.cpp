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
    VCLinkerToolMaker           link;
    VCPreBuildEventToolMaker    pre_build;
    PreferredPathMaker          preferred_path;
    IncludeStdAfxMaker          add_include_StdAfx_for_cpp;
    GenerateStdAfxMaker         generate_StdAfx;

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        VisualStudioProjectPtr project = projects[i];
        FilesHelperPtr files_helper = project->m_files_helper;
        const std::string& configuration_type = project->m_project_helper->get_configuration_type();

        if ( ( configuration_type != "1" &&  configuration_type != "4" ) || // 1: Application (.exe), 4: Static Library (.lib)
             ( files_helper->has_file( "StdAfx.h" ) || files_helper->has_file( "StdAfx.cpp" ) ) )
        {
            continue;
        }

        std::cout << project->m_path.string() << std::endl;

        compile.make_project( project );
        preferred_path.make_project( project );
        add_include_StdAfx_for_cpp.make_project( project );

        if ( "4" == configuration_type ) // 4: Static Library (.lib)
        {
            pre_build.make_project( project );
        }

        if ( "1" == configuration_type ) // 1: Application (.exe)
        {
            link.make_project( project );
            generate_StdAfx.make_project( project );
        }
    }

    for ( size_t i = 0; i < projects.size(); ++i )
    {
        projects[i]->save();
    }
}
