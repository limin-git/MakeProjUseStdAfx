#pragma once
#include "CommonDefs.h"


class FilesHelper
{
public:

    FilesHelper( VisualStudioProject* project, const std::string& configuration_name = "Debug" );

    bool is_exist( const path& extension = path() );
    bool has_file( const std::string& file_name );
    const std::vector<path>& get_paths() { return m_paths; }
    std::vector<path> get_paths_by_extension( const path& extension );

private:

    void get_paths_from_files( FilesPtr files, std::vector<path>& paths );
    void get_paths_from_filters_files( const FilterPtrList& filters, const FilePtrList& files, std::vector<path>& paths );
    void get_path_from_filter( FilterPtr filter, std::vector<path>& paths );
    path get_path_from_file( FilePtr file );

public:

    VisualStudioProject* m_project;
    std::vector<path> m_paths;
    std::string m_configuration_name;
};
