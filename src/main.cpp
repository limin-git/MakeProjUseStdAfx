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
#include "CommonDefs.h"


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
    VisualStudioProjectPtrList projects;

    if ( p.extension() == _sln )
    {
        Solution solution( p );
        const std::vector<path>&  project_paths = solution.get_project_paths();

        for ( size_t i = 0; i < project_paths.size(); ++i )
        {
            VisualStudioProjectPtr project( new VisualStudioProject( project_paths[i] ) );
            projects.push_back( project );
        }
    }
    else if ( p.extension() == _vcproj )
    {
        VisualStudioProjectPtr project( new VisualStudioProject( p ) );
        projects.push_back( project );
    }
    else
    {
        std::cout << "only support " << _sln.string() << ", " << _vcproj.string() << std::endl;
        return;
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

        if ( ( configuration_type != ct_Application &&  configuration_type != ct_DynamicLibrary &&  configuration_type != ct_StaticLibrary ) ||
             ( files_helper->has_file( StdAfx_h ) || files_helper->has_file( StdAfx_cpp ) ) )
        {
            continue;
        }

        std::cout << project->m_path.string() << std::endl;

        compile.make_project( project );
        preferred_path.make_project( project );
        add_include_StdAfx_for_cpp.make_project( project );

        if ( ct_StaticLibrary == configuration_type )
        {
            pre_build.make_project( project );
        }

        if ( ct_Application == configuration_type || ct_DynamicLibrary == configuration_type )
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
