#pragma once
#include "CommonDefs.h"
#include "ToolMaker.h"


class VCCLCompilerToolMaker : ToolMaker
{
public:

    VCCLCompilerToolMaker()
        : ToolMaker( "VCCLCompilerTool" )
    {
    }

    virtual void make_project( VisualStudioProjectPtr project, const std::string& configuration_name = "Debug" );

private:

    void make_PreprocessorDefinitions();
    void make_AdditionalOptions();
    void make_AdditionalIncludeDirectories();
    void make_PrecompiledHeaderFile();
    void make_UsePrecompiledHeader();
    void make_WarningLevel();
    void make_BrowseInformation();
    void make_MinimalRebuild();
};
