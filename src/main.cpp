#include "StdAfx.h"
#include "Vcproj.h"
#include "Solution.h"
#include "Utility.h"
#include "VisualStudioProject.h"
#include "VCCLCompilerToolMaker.h"
#include "VCPreBuildEventToolMaker.h"


void main(int argc, char* argv[])
{
#if 0
    Solution sln( "C:\\Code\\PchBuild\\code\\tools\\build\\tools.build.Build_Base_Core.sln" );
    sln.make_solution();
    return;
#endif

#if 1
    std::string p = "C:\\Code\\PchBuild\\code\\transactive\\core\\alarm\\core.alarm.TA_Alarm.vcproj";
    VisualStudioProjectPtr vp( new VisualStudioProject(p) );

    VCCLCompilerToolMaker m1( vp );
    m1.make_all();
    VCPreBuildEventToolMaker m2( vp );
    m2.make_all();

    std::string out = vp->generate_visual_studio_project();
    Utility::write_string_to_file( out, "c:\\temp\\test.txt" );




#endif

    //Vcproj p( "C:\\My Document\\Code\\C++\\MakeProjUseStdAfx\\MakeProjUseStdAfx.vcproj" );
    //Vcproj p( "C:\\Code\\FZL1_TIP\\TA_BASE\\code\\transactive\\app\\notification_service\\app.notification_service.NotificationServiceAgent.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\alarm\\core.alarm.TA_Alarm.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\uuid\\core.uuid.TA_Uuid.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\threads\\core.threads.TA_Thread.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\corba\\IDL\\core.corba.idl.TA_CorbaDef.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\versioning\\core.versioning.TA_Version.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\transactive\\core\\data_access_interface\\soe\\core.dai.soe.vcproj" );
    //Vcproj p( "C:\\Code\\PchBuild\\code\\tools\\build\\core_library\\tools.build.core_library.TA_Base_Core.vcproj" );
    //p.make_project();

    //path p = "../../;..\..\..\cots\ACE\6_0_4\ACE_wrappers;..\..\..\cots\boost\boost_1_39_0;..\..\..\cots\omniORB\omniORB_4.1.6\include";
    //p.make_preferred();
    //std::cout << "\t" << p.string() << std::endl;
}
