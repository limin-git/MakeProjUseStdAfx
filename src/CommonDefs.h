#pragma once


using namespace boost::filesystem;

typedef std::pair<std::string, std::string> Option;
typedef std::list<Option> OptionList;

class VisualStudioProject;
typedef boost::shared_ptr<VisualStudioProject> VisualStudioProjectPtr;
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
class Tool;
typedef boost::shared_ptr<Tool> ToolPtr;
class References;
typedef boost::shared_ptr<References> ReferencesPtr;
class Files;
typedef boost::shared_ptr<Files> FilesPtr;
class Filter;
typedef boost::shared_ptr<Filter> FilterPtr;
class File;
typedef boost::shared_ptr<File> FilePtr;
class FileConfiguration;
typedef boost::shared_ptr<FileConfiguration> FileConfigurationPtr;
class Globals;
typedef boost::shared_ptr<Globals> GlobalsPtr;

class OptionListHelper;
typedef boost::shared_ptr<OptionListHelper> OptionListHelperPtr;
