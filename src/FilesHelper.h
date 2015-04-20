#pragma once
#include "CommonDefs.h"


class FilesHelper
{
public:

    FilesHelper( VisualStudioProjectPtr project, const std::string& configuration_name = "Win32" );

    bool is_exist();
    const std::vector<path>& get_paths() { return m_paths; }
    std::vector<path> get_paths_by_extension( const path& extension );

private:

    void get_paths_from_files( FilesPtr files, std::vector<path>& paths );
    void get_path_from_filter( FilterPtr filter, std::vector<path>& paths );
    path get_path_from_file( FilePtr file );

public:

    VisualStudioProjectPtr m_project;
    std::vector<path> m_paths;
    std::string m_configuration_name;
};
