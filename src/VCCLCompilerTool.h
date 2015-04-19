#pragma once
#include "Tool.h"


class VCCLCompilerTool : public Tool
{
public:

    VCCLCompilerTool( Vcproj* project, const std::string& str, size_t pos )
        : Tool( project, str, pos )
    {
    }

    virtual void make_tool();

    void make_PreprocessorDefinitions();
    void make_AdditionalOptions();
    void make_AdditionalIncludeDirectories();
    void make_PrecompiledHeaderFile();
    void make_UsePrecompiledHeader();
    void make_WarningLevel();
    void make_BrowseInformation();
    void make_MinimalRebuild();
};
