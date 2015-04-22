#include "StdAfx.h"
#include "ProjectHelper.h"
#include "VisualStudioProject.h"
#include "Files.h"
#include "File.h"
#include "Filter.h"
#include "FileConfiguration.h"
#include "Utility.h"
#include "Configurations.h"
#include "Configuration.h"
#include "OptionListHelper.h"


ProjectHelper::ProjectHelper( VisualStudioProject* project, const std::string& configuration_name )
    : m_project( project ),
      m_configuration_name( configuration_name )
{
}


const std::string& ProjectHelper::get_configuration_type()
{
    if ( m_project && m_project->m_configurations )
    {
        ConfigurationPtrList& configurations = m_project->m_configurations->m_configurations;

        for ( size_t i = 0; i < configurations.size(); ++i )
        {
            OptionListHelper configuration_options( &configurations[i]->m_options );

            if ( configuration_options.get_option_value( "Name" ) == m_configuration_name + "|Win32" )
            {
                // 0: MakeFile
                // 1: Application (.exe)
                // 2: Dynamic Library (.dll)
                // 4: Static Library (.lib)
                //10: Utility
                return configuration_options.get_option_value( "ConfigurationType" );
            }
        }
    }

    static std::string empty;
    return empty;
}
