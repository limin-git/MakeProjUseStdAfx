#pragma once


using namespace boost::filesystem;

typedef std::pair<std::string, std::string> Option;
typedef std::list<Option> OptionList;

class VisualStudioProject;
typedef boost::shared_ptr<VisualStudioProject> VisualStudioProjectPtr;
typedef std::vector<VisualStudioProjectPtr> VisualStudioProjectPtrList;
class Platforms;
typedef boost::shared_ptr<Platforms> PlatformsPtr;
class Platform;
typedef boost::shared_ptr<Platform> PlatformPtr;
class ToolFiles;
typedef boost::shared_ptr<ToolFiles> ToolFilesPtr;
class Configurations;
typedef boost::shared_ptr<Configurations> ConfigurationsPtr;
class Configuration;
typedef boost::shared_ptr<Configuration> ConfigurationPtr;
typedef std::vector<ConfigurationPtr> ConfigurationPtrList;
class Tool;
typedef boost::shared_ptr<Tool> ToolPtr;
typedef std::vector<ToolPtr> ToolPtrList;
class References;
typedef boost::shared_ptr<References> ReferencesPtr;
class Files;
typedef boost::shared_ptr<Files> FilesPtr;
class Filter;
typedef boost::shared_ptr<Filter> FilterPtr;
typedef std::vector<FilterPtr> FilterPtrList;
class File;
typedef boost::shared_ptr<File> FilePtr;
class FileConfiguration;
typedef std::vector<FilePtr> FilePtrList;
typedef boost::shared_ptr<FileConfiguration> FileConfigurationPtr;
typedef std::vector<FileConfigurationPtr> FileConfigurationPtrList;
class Globals;
typedef boost::shared_ptr<Globals> GlobalsPtr;

class OptionListHelper;
typedef boost::shared_ptr<OptionListHelper> OptionListHelperPtr;

class FilesHelper;
typedef boost::shared_ptr<FilesHelper> FilesHelperPtr;

class ProjectHelper;
typedef boost::shared_ptr<ProjectHelper> ProjectHelperPtr;


const path _sln                             = ".sln";
const path _vcproj                          = ".vcproj";
const path StdAfx_h                         = "StdAfx.h";
const path StdAfx_cpp                       = "StdAfx.cpp";
#define ct_MakeFile                         "0"
#define ct_Application                      "1"
#define ct_DynamicLibrary                   "2"
#define ct_StaticLibrary                    "4"
#define ct_Utility                          "10"
