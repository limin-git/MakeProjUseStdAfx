#pragma  once
#include "CommonDefs.h"


class GenerateStdAfxMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );
    void generate_StdAfx_h();
    void init_includes();
    void set_include( const std::string& include );
    void collect_includes( std::set<path>& project_includes );

    void add_stdafx_to_project();

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    FilesHelperPtr m_files_helper;
    std::vector< std::pair<std::string, bool> > m_includes;
};
