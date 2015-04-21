#pragma once
#include "CommonDefs.h"


class Solution
{
public:

    Solution( const path& p );
    
    const std::vector<path>& get_project_paths() { return m_projects; }

private:

    void extract_projects();

public:

    path m_path;
    std::string m_str;
    path m_current_path;
    std::vector<path> m_projects;
};
