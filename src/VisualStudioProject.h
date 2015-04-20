#pragma once
#include "CommonDefs.h"


class VisualStudioProject
{
public:

    VisualStudioProject( const path& p );

    bool is_changed();
    std::string generate_visual_studio_project();
    void save();

public:

    path m_path;
    path m_current_path;
    std::string m_str;
    std::string m_version;
    OptionList m_options;
    PlatformsPtr m_platforms;
    ToolFilesPtr m_tool_files;
    ConfigurationsPtr m_configurations;
    ReferencesPtr m_references;
    FilesPtr m_files;
    GlobalsPtr m_globals;
};
