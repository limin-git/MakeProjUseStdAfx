#pragma once
#include "CommonDefs.h"


class VCCLCompilerToolMaker
{
public:

    void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    void make_PreprocessorDefinitions();
    void make_AdditionalOptions();
    void make_AdditionalIncludeDirectories();
    void make_PrecompiledHeaderFile();
    void make_UsePrecompiledHeader();
    void make_WarningLevel();
    void make_BrowseInformation();
    void make_MinimalRebuild();

private:

    void initialize( VisualStudioProjectPtr project, const std::string& configuration_name );

public:

    VisualStudioProjectPtr m_project;
    std::string m_configuration_name;
    ToolPtr m_tool;
    OptionListHelperPtr m_tool_options;
};
