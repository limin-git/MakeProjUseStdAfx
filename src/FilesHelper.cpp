#include "StdAfx.h"
#include "FilesHelper.h"
#include "VisualStudioProject.h"
#include "Files.h"
#include "File.h"
#include "Filter.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "OptionListHelper.h"


FilesHelper::FilesHelper( VisualStudioProject* project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
    if ( project != NULL )
    {
        get_paths_from_files( m_project->m_files, m_paths );
        //Utility::output_paths( std::cout, m_paths );
    }
}


bool FilesHelper::is_exist( const path& extension )
{
    if ( extension.empty() )
    {
        for ( size_t i = 0; i < m_paths.size(); ++i )
        {
            if ( false == boost::filesystem::exists( m_paths[i] ) )
            {
                return false;
            }
        }

        return ( false == m_paths.empty() );
    }
    else
    {
        std::vector<path> paths = get_paths_by_extension( extension );

        for ( size_t i = 0; i < paths.size(); ++i )
        {
            if ( false == boost::filesystem::exists( m_paths[i] ) )
            {
                return false;
            }
        }

        return ( false == paths.empty() );
    }
}


bool FilesHelper::has_file( const std::string& file_name )
{
    std::string lhs = boost::to_lower_copy( file_name );

    for ( size_t i = 0; i < m_paths.size(); ++i )
    {
        std::string rhs = boost::to_lower_copy( m_paths[i].filename().string() );

        if ( lhs == rhs )
        {
            return true;
        }
    }

    return false;
}


std::vector<path> FilesHelper::get_paths_by_extension( const path& extension )
{
    std::vector<path> paths;

    for ( size_t i = 0; i < m_paths.size(); ++i )
    {
        if ( m_paths[i].extension() == extension )
        {
            paths.push_back( m_paths[i] );
        }
    }

    return paths;
}


std::vector<path> FilesHelper::get_source_code_paths()
{
    const char* extensions[] = { ".c", ".cpp", ".cxx", ".cc", ".h", ".hpp", ".hxx", ".hh", ".inl" };
    size_t cnt = sizeof(extensions) / sizeof(char*);

    std::vector<path> paths;

    for ( size_t i = 0; i < m_paths.size(); ++i )
    {
        for ( size_t j = 0; j < cnt; ++j )
        {
            if ( m_paths[i].extension() == extensions[j] )
            {
                paths.push_back( m_paths[i] );
                break;
            }
        }
    }

    return paths;
}


void FilesHelper::get_paths_from_files( FilesPtr files, std::vector<path>& paths )
{
    if ( files )
    {
        get_paths_from_filters_files( files->m_filters, files->m_files, paths );
    }
}


void FilesHelper::get_paths_from_filters_files( const FilterPtrList& filters, const FilePtrList& files, std::vector<path>& paths )
{
    for ( size_t i = 0; i < filters.size(); ++i )
    {
        get_path_from_filter( filters[i], paths );
    }

    for ( size_t i = 0; i < files.size(); ++i )
    {
        path p = get_path_from_file( files[i] );

        if ( false == p.empty() )
        {
            paths.push_back( p );
        }
    }
}


void FilesHelper::get_path_from_filter( FilterPtr filter, std::vector<path>& paths )
{
    if ( filter )
    {
        get_paths_from_filters_files( filter->m_filters, filter->m_files, paths );
    }
}


path FilesHelper::get_path_from_file( FilePtr file )
{
    if ( ! file )
    {
        return path();
    }

    const FileConfigurationPtrList& file_configurations = file->m_file_configurations;

    for ( size_t i = 0; i < file_configurations.size(); ++i )
    {
        OptionListHelper file_configuration_options( &file_configurations[i]->m_options );

        if ( file_configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
        {
            if ( file_configuration_options.get_option_value( "ExcludedFromBuild" ) == "true" )
            {
                return path();
            }
        }
    }

    OptionListHelper file_options( &file->m_options );
    return path( boost::filesystem::system_complete( m_project->m_current_path / file_options.get_option_value( "RelativePath" ) ) );
}
