#pragma once
#include "CommonDefs.h"


class IncludeStdAfxMaker
{
public:

    IncludeStdAfxMaker( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    virtual void make_all();
    void add_include_StdAfx_h();

public:

    VisualStudioProjectPtr m_project;
    FilesPtr m_files;
    std::string m_configuration_name;
    FilesHelperPtr m_helper;
};
