#include "StdAfx.h"
#include "GenerateStdAfxMaker.h"
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
#include "FileCollector.h"
#include "ProjectHelper.h"


void GenerateStdAfxMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;
    
    if ( m_project )
    {
        m_files_helper = m_project->m_files_helper;
    }

    init_includes();
}


void GenerateStdAfxMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    initialize( project, configuration_name );

    generate_StdAfx_h();
}


void GenerateStdAfxMaker::init_includes()
{
    static const char* includes[] =
    {
        // atlmfc
        "afx.h", "afxadv.h", "afxcoll.h", "afxconv.h", "afxcview.h", "afxdlgs.h", "afxdllx.h", "afxmt.h", "afxpriv.h", "atlbase.h", "atlcom.h", "atlcomcli.h",
        "atlconv.h", "statreg.h", "afxcmn.h", "afxdao.h", "afxdb.h", "afxdisp.h", "afxdtctl.h", "afxext.h", "afxodlgs.h", "afxole.h", "afxtempl.h", "afxwin.h",

        // vc60
        "winsock2.h", "winsock.h", "windows.h", "windowsx.h", "winnt.h", "winspool.h", "windows.h", "winbase.h", "winreg.h", "mmsystem.h", "objbase.h", "sql.h", "sqlext.h",
        "cderr.h", "commdlg.h", "guiddef.h", "mmreg.h", "oaidl.h", "ocidl.h", "ole2.h", "rpc.h", "rpcndr.h", "initguid.h", "oleauto.h", "tlhelp32.h", "vdmdbg.h", "winuser.h",

        // std_h
        "limits.h", "assert.h", "climits", "ctype.h", "errno.h", "fcntl.h", "float.h", "io.h", "math.h", "mbstring.h", "memory.h", "process.h",
        "signal.h", "stdarg.h", "stddef.h", "stdexcept", "stdio.h", "stdlib.h", "string.h", "tchar.h", "time.h", "wchar.h", "comdef.h", "crtdbg.h", "eh.h",
        "limits.h", "memory.h", "new.h", "time.h", "typeinfo.h", "xmemory", "yvals.h", "sys/stat.h", "sys/timeb.h", "sys/types.h", "direct.h", "comutil.h", "xstring",

        // cstd
        "cerrno", "cstring", "ctime", "cassert", "cctype", "cmath", "cfloat", "cstddef", "cstdio", "cstdlib",

        // stl
        "string", "vector", "map", "list", "set", "queue", "deque", "stack", "bitset", "iostream", "istream", "ostream", "strstream", "sstream", "fstream",
        "streambuf", "iomanip", "ios", "iosfwd", "algorithm", "memory", "new", "typeinfo", "utility", "iterator", "locale", "complex", "limits", "functional", "exception",

        // boost
        //"boost/date_time/posix_time/posix_time.hpp" // error LNK2001
        "boost/shared_ptr.hpp", "boost/smart_ptr.hpp", "boost/regex.hpp", "boost/cast.hpp", "boost/config.hpp", "boost/lexical_cast.hpp", "boost/tokenizer.hpp",
        "boost/unordered_map.hpp", "boost/format.hpp", "boost/filesystem/exception.hpp", "boost/filesystem/fstream.hpp", "boost/filesystem/operations.hpp", "boost/filesystem/path.hpp",
        "boost/interprocess/allocators/allocator.hpp", "boost/interprocess/managed_windows_shared_memory.hpp", "boost/interprocess/sync/named_mutex.hpp", "boost/multi_index/composite_key.hpp",
        "boost/multi_index/hashed_index.hpp", "boost/multi_index/member.hpp", "boost/multi_index/ordered_index.hpp", "boost/multi_index_container.hpp", "boost/algorithm/string.hpp",

        // ace
        "ace/ACE.h", "ace/OS.h", "ace/Singleton.h", "ace/Future.h", "ace/Guard_T.h", "ace/INET_Addr.h", "ace/SOCK_Connector.h", "ace/Object_Manager.h", "ace/Log_Msg.h", "ace/Tokenizer_T.h",
        "ace/Message_Block.h", "ace/Method_Request.h", "ace/OS_NS_sys_time.h", "ace/Date_Time.h", "ace/Time_Value.h", "ace/Timer_Heap.h", "ace/Timer_Queue.h", "ace/Timer_Queue_Adapters.h",
        "ace/os_include/netinet/os_tcp.h", "ace/Task.h", "ace/Thread_Manager.h", "ace/Synch.h", "ace/Mutex.h", "ace/RW_Mutex.h", "ace/Thread_Semaphore.h", "ace/Containers_T.h", "ace/Unbounded_Queue.h",
        "ace/RB_Tree.h", "ace/Functor.h", "ace/Null_Mutex.h", "ace/SString.h", "ace/Malloc_T.h", "ace/Memory_Pool.h", "ace/Activation_Queue.h", "ace/Global_Macros.h", "ace/SOCK_Stream.h",

        // omniorb
        "omnithread.h", "omniORB4/CORBA.h", "omniORB4/IOP_C.h", "omniORB4/IOP_S.h", "omniORB4/callDescriptor.h", "omniORB4/callHandle.h", "omniORB4/objTracker.h", "omniORB4/internal/orbParameters.h",
        "omniNotify.h", "COS/CosNotifyChannelAdmin.hh", "COS/CosNotifyFilter.hh", "COS/AttNotification.hh", "COS/CosEventChannelAdmin.hh", "COS/CosEventComm.hh", "COS/CosNotification.hh", "COS/CosNotifyComm.hh"
    };

    //m_includes.clear();

    size_t cnt = sizeof(includes) / sizeof(char*);

    for ( size_t i = 0; i < cnt; ++i )
    {
        m_includes.push_back( std::make_pair( includes[i], false ) );
    }
}


void GenerateStdAfxMaker::set_include( const std::string& include )
{
    for ( size_t i = 0; i < m_includes.size(); ++i )
    {
        const std::string& s = m_includes[i].first;

        if ( boost::contains( s, "/" ) )
        {
            if ( boost::iends_with( s, include ) )
            {
                m_includes[i].second = true;
                break;
            }
        }
        else
        {
            if ( s == include )
            {
                m_includes[i].second = true;
                break;
            }
        }
    }
}


void GenerateStdAfxMaker::generate_StdAfx_h()
{
    path std_afx_h = m_project->m_current_path / "src\\StdAfx.h";
    path std_afx_cpp = m_project->m_current_path / "src\\StdAfx.cpp";

    if ( boost::filesystem::exists( std_afx_h ) || boost::filesystem::exists( std_afx_cpp ) )
    {
        return;
    }

    std::vector<path> project_includes;
    collect_includes( project_includes );

    for ( std::vector<path>::iterator it = project_includes.begin(); it != project_includes.end(); ++it )
    {
        //std::cout << it->filename().string() << std::endl;
        set_include( boost::trim_copy( it->filename().string() ) );
    }

    std::stringstream strm;

    strm << "#pragma once \n\n\n";

    for ( size_t i = 0; i < m_includes.size(); ++i )
    {
        if ( true == m_includes[i].second )
        {
            strm << "#include <" << m_includes[i].first << ">\n";
        }
    }

    //std::cout << strm.rdbuf() << std::endl;

    if ( ! m_project )
    {
        std::cout << "\t" << "cannot generate StdAfx.h, because cannot find path." << std::endl;
        return;
    }

    path p = m_project->m_current_path / "src";

    if ( ! boost::filesystem::exists( p ) )
    {
        p = m_project->m_current_path;
    }

    Utility::write_string_to_file( strm.str(), p / "StdAfx.h" );
    std::cout << "+ " << ( p / "StdAfx.h" ).string() << std::endl;
    strm.str( "#include \"StdAfx.h\"\n" );
    Utility::write_string_to_file( strm.str(), p / "StdAfx.cpp" );
    std::cout << "+ " << ( p / "StdAfx.cpp" ).string() << std::endl;
}


void GenerateStdAfxMaker::collect_includes( std::vector<path>& project_includes )
{
    if ( ! m_project )
    {
        return;
    }

    std::vector<path> files = m_project->m_files_helper->get_paths_by_extension( ".cpp" );
    const path& current_path = m_project->m_current_path;
    const std::vector<path>& additional_directories = m_project->m_project_helper->get_additional_include_directories();
    std::vector< std::set<path> > file_includes( files.size() );
    std::vector<boost::shared_ptr<boost::thread> > threads;

    for ( size_t i = 0; i < files.size(); ++i )
    {
        boost::shared_ptr<boost::thread> t = FileCollector::create_FileCollectorThread( file_includes[i], files[i], current_path, additional_directories );
        threads.push_back( t );
    }

    for ( size_t i = 0; i < threads.size(); ++i )
    {
        threads[i]->join();
    }

    for ( size_t i = 0; i < file_includes.size(); ++i )
    {
        project_includes.insert( project_includes.begin(), file_includes[i].begin(), file_includes[i].end() );
    }
}
