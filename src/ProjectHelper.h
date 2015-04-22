#pragma once
#include "CommonDefs.h"


class ProjectHelper
{
public:

    ProjectHelper( VisualStudioProject* project, const std::string& configuration_name = "Debug" );

    const std::string& get_configuration_type();

public:

    VisualStudioProject* m_project;
    std::string m_configuration_name;
};
