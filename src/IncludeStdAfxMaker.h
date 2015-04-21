#pragma once
#include "CommonDefs.h"


class IncludeStdAfxMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );
    void add_include_StdAfx_h();

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    FilesHelperPtr m_files_helper;
};
