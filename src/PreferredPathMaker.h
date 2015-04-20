#pragma once
#include "CommonDefs.h"


class PreferredPathMaker
{
public:

    PreferredPathMaker( VisualStudioProjectPtr project );

    void make_all();
    void make_preferred_path();

public:

    VisualStudioProjectPtr m_project;
};
