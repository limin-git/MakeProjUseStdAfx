#pragma once
#include "CommonDefs.h"


class ProjectHelper
{
public:

    ProjectHelper( VisualStudioProject* project, const std::string& configuration_name = "Debug" );

    const std::string& get_configuration_type();

    const std::vector<path>& get_additional_include_directories();

public:

    VisualStudioProject* m_project;
    std::string m_configuration_name;
    std::string m_configuration_name_Win32;
    std::vector<path> m_additional_include_directories;
};
