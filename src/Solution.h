#pragma once
#include <boost/filesystem.hpp>


using namespace boost::filesystem;


class Solution
{
public:

    Solution( const path& p );

    void make_solution();

private:

    void extract_projects();

public:

    path m_path;
    std::string m_str;
    path m_current_path;
    std::vector<path> m_projects;
};
