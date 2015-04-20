#pragma once
#include "CommonDefs.h"


class VCCLCompilerToolMaker
{
public:

    VCCLCompilerToolMaker( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

    void make_all();
    void make_PreprocessorDefinitions();
    void make_AdditionalOptions();
    void make_AdditionalIncludeDirectories();
    void make_PrecompiledHeaderFile();
    void make_UsePrecompiledHeader();
    void make_WarningLevel();
    void make_BrowseInformation();
    void make_MinimalRebuild();

public:

    VisualStudioProjectPtr m_project;
    ToolPtr m_tool;
    OptionListHelperPtr m_helper;
    std::string m_configuration_name;
};
