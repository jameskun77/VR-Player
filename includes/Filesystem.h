#pragma once

#include <string>
#include <cstdlib>

#include "root_directory.h"

class FileSystem
{
private:
	typedef std::string (*Builder) (const std::string& path);
	
public:
	static std::string getPath(const std::string& path)
	{
		static std::string(*pathBuilder)(const std::string&) = getPathBuilder();
		return (*pathBuilder)(path);
	}
	
private:
	static Builder getPathBuilder()
	{
		return &FileSystem::getPathRelative;
	}
	
	static std::string getPathRelative(const std::string& path)
	{
		return source_root + std::string("/") + path;
	}
};

