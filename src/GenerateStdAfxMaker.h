#pragma  once
#include "CommonDefs.h"


class GenerateStdAfxMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );
    void init_standared_includes();
    void generate_StdAfx();
    void set_include( const path& include );
    void collect_includes( std::set<path>& project_includes );

    void add_StdAfx_files_to_vcproj();

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    std::string m_configuration_name_Win32;
    FilesHelperPtr m_files_helper;
    std::vector< std::pair<path, bool> > m_includes;
};
