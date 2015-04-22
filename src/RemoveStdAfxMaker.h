#pragma once
#include "CommonDefs.h"


class RemoveStdAfxMaker
{
public:

    void make_project( VisualStudioProjectPtr project );

private:

    void initialize( VisualStudioProjectPtr project );
    void remove_stdafx_cpp();

public:


    VisualStudioProjectPtr m_project;
    FilesPtr m_files;
};
