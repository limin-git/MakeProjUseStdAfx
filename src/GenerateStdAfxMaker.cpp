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
#include "IncludeCollector.h"
#include "ProjectHelper.h"


void GenerateStdAfxMaker::initialize( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    m_project = project;
    m_configuration_name = configuration_name;

    if ( m_project )
    {
        m_files_helper = m_project->m_files_helper;
    }

    init_standared_includes();
}


void GenerateStdAfxMaker::make_project( VisualStudioProjectPtr project, const std::string& configuration_name )
{
    initialize( project, configuration_name );

    generate_StdAfx();
}


void GenerateStdAfxMaker::init_standared_includes()
{
    static const path includes[] =
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
    size_t cnt = sizeof(includes) / sizeof(path);

    m_includes.clear();

    for ( size_t i = 0; i < cnt; ++i )
    {
        m_includes.push_back( std::make_pair( includes[i], false ) );
    }
}


void GenerateStdAfxMaker::set_include( const path& include )
{
    for ( size_t i = 0; i < m_includes.size(); ++i )
    {
        const path& p = m_includes[i].first;

        if ( p == include )
        {
            m_includes[i].second = true;
            break;
        }
        else if ( p.has_parent_path() )
        {
            if ( p.filename() == include.filename() )
            {
                m_includes[i].second = true;
                break;
            }
        }
    }
}


void GenerateStdAfxMaker::generate_StdAfx()
{
    if ( ! m_project )
    {
        return;
    }

    path std_afx_h = m_project->m_current_path / "src\\StdAfx.h";
    path std_afx_cpp = m_project->m_current_path / "src\\StdAfx.cpp";

    // 1. check existing
    if ( boost::filesystem::exists( std_afx_h ) || boost::filesystem::exists( std_afx_cpp ) )
    {
        return;
    }

    // 2. collece includes
    std::set<path> project_includes;

    collect_includes( project_includes );

    // 3. mark standared includes
    for ( std::set<path>::iterator it = project_includes.begin(); it != project_includes.end(); ++it )
    {
        //std::cout << it->filename().string() << std::endl;
        set_include( boost::trim_copy( it->filename().string() ) );
    }

    // 4 generate StdAfx
    std::stringstream strm;

    // 4.1
    strm << "#pragma once" << std::endl;

    // 4.2 generate standared includes
    for ( size_t i = 0; i < m_includes.size(); ++i )
    {
        if ( true == m_includes[i].second )
        {
            strm << "#include <" << m_includes[i].first.string() << ">" << std::endl;
        }
    }

    // 4.3 generate transactive includes

    // 4.3.1 remove files of this own project
    const std::vector<path>& headers = m_project->m_files_helper->get_paths();

    for ( size_t i = 0; i < headers.size(); ++i )
    {
        // TODO: ignore case
        project_includes.erase( headers[i] );
    }

    // 4.3.2 add transactive includes in order of app-bus-core.
    // TODO: add cots
    const char* folders[] = { "\\app\\", "\\bus", "\\core\\" };
    size_t size = sizeof(folders) / sizeof(char*);

    for ( size_t i = 0; i < size; ++i )
    {
        for ( std::set<path>::iterator it = project_includes.begin(); it != project_includes.end(); ++it )
        {
            std::string s = it->string();
            size_t pos = s.find( folders[i] );

            if ( pos != std::string::npos )
            {
                strm << "#include \"" << s.substr( pos + 1 ) << "\"" << std::endl;
            }
        }
    }

    //std::cout << strm.rdbuf() << std::endl;

    // 5. generate StdAfs.h, StdAfx.cpp
    Utility::write_string_to_file( strm.str(), std_afx_h );
    std::cout << "\t" << "+ " << std_afx_h.string() << std::endl;
    strm.str( "#include \"StdAfx.h\"\n" );
    Utility::write_string_to_file( strm.str(), std_afx_cpp );
    std::cout << "\t" << "+ " << std_afx_cpp.string() << std::endl;

    // 6. add StdAfx.h, StdAfx.cpp to .vcproj
    add_StdAfx_files_to_vcproj();
}


void GenerateStdAfxMaker::collect_includes( std::set<path>& project_includes )
{
    const std::vector<path> files = m_project->m_files_helper->get_source_code_paths(); // donot get all, especially .rc
    const path& current_path = m_project->m_current_path;
    const std::vector<path>& additional_directories = m_project->m_project_helper->get_additional_include_directories();
    std::vector< std::set<path> > file_includes( files.size() );
    std::vector<boost::shared_ptr<boost::thread> > threads;

    for ( size_t i = 0; i < files.size(); ++i )
    {
        boost::shared_ptr<boost::thread> t = IncludeCollector::create_FileCollectorThread( file_includes[i], files[i], current_path, additional_directories );
        threads.push_back( t );
    }

    for ( size_t i = 0; i < threads.size(); ++i )
    {
        threads[i]->join();
    }

    for ( size_t i = 0; i < file_includes.size(); ++i )
    {
        project_includes.insert( file_includes[i].begin(), file_includes[i].end() );
    }
}


void GenerateStdAfxMaker::add_StdAfx_files_to_vcproj()
{
    std::string& s = m_project->m_files->m_str;
    boost::smatch m;

    if ( ! boost::regex_search( s, m, boost::regex( "(?x) ^ ([\t]+) <Filter \\s+ Name= \" (src|Source[ ]Files) .+? \\n (\\1) </Filter>" ) ) )
    {
        return;
    }

    std::string indt = m.str(1);
    size_t pos = m.position(3);
    std::stringstream strm;

    indt += "\t";

    strm
        << indt << "<File\n"
        << indt << "	RelativePath=\".\\src\\StdAfx.cpp\"\n"
        << indt << "	>\n"
        << indt << "	<FileConfiguration\n"
        << indt << "		Name=\"" << m_configuration_name << "|Win32\"\n"
        << indt << "		>\n"
        << indt << "		<Tool\n"
        << indt << "			Name=\"VCCLCompilerTool\"\n"
        << indt << "			UsePrecompiledHeader=\"1\"\n"
        << indt << "		/>\n"
        << indt << "	</FileConfiguration>\n"
        << indt << "</File>\n"
        << indt << "<File\n"
        << indt << "	RelativePath=\".\\src\\StdAfx.h\"\n"
        << indt << "	>\n"
        << indt << "</File>\n"
        ;

    s.insert( pos, strm.str() );
    m_project->m_files->set_changed();
}
