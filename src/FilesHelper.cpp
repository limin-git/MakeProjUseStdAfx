#include "StdAfx.h"
#include "FilesHelper.h"
#include "VisualStudioProject.h"
#include "Files.h"
#include "File.h"
#include "Filter.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "OptionListHelper.h"


FilesHelper::FilesHelper( VisualStudioProjectPtr project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
    get_paths_from_files( m_project->m_files, m_paths );
}


bool FilesHelper::is_exist()
{
    for ( size_t i = 0; i < m_paths.size(); ++i )
    {
        if ( false == boost::filesystem::exists( m_paths[i] ) )
        {
            return false;
        }
    }

    return true;
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


void FilesHelper::get_paths_from_files( FilesPtr files, std::vector<path>& paths )
{
    for ( size_t i = 0; i < files->m_filters.size(); ++i )
    {
        get_path_from_filter( files->m_filters[i], paths );
    }

    for ( size_t i = 0; i < files->m_files.size(); ++i )
    {
        path p = get_path_from_file( files->m_files[i] );

        if ( false == p.empty() )
        {
            paths.push_back( p );
        }
    }
}


void FilesHelper::get_path_from_filter( FilterPtr filter, std::vector<path>& paths )
{
    for ( size_t i = 0; i < filter->m_filters.size(); ++i )
    {
        get_path_from_filter( filter->m_filters[i], paths );
    }

    for ( size_t i = 0; i < filter->m_files.size(); ++i )
    {
        path p = get_path_from_file( filter->m_files[i] );

        if ( false == p.empty() )
        {
            paths.push_back( p );
        }
    }
}


path FilesHelper::get_path_from_file( FilePtr file )
{
    const std::vector<FileConfigurationPtr>& file_configurations = file->m_file_configurations;

    for ( size_t i = 0; i < file_configurations.size(); ++i )
    {
        OptionListHelper helper( &file_configurations[i]->m_options );

        if ( helper.get_option( "Name" ).second == ( m_configuration_name + "|Win32" ) )
        {
            if ( helper.get_option( "ExcludedFromBuild" ).second == "true" )
            {
                return path();
            }
        }
    }

    OptionListHelper helper( &file->m_options );
    return path( m_project->m_current_path / helper.get_option( "RelativePath" ).second );
}
