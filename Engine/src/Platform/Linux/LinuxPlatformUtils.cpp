#ifdef LINUX

#include "pch.h"
#include "Engine/Utils/FileDialogs.h"

namespace Engine
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
		char file_path[256]{ 0 };
		std::string filter_str = filter;
		filter_str.insert(0, "zenity --file-selection  --title=\"Select a File\" --file-filter=");
		FILE* file = popen(filter_str.c_str(), "r");

		bool opened = fgets(file_path, 1024, file);
		if (opened)
		{
			std::string path = std::string(file_path);
			path.erase(std::find(path.begin(), path.end(), '\n'), path.end());
			return path;
		}
		pclose(file);
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		char file_path[256]{ 0 };
		std::string filter_str = filter;
		filter_str.insert(0, "zenity --file-selection --save --confirm-overwrite --title=\"Save As\"");
		FILE* file = popen(filter_str.c_str(), "r");

		bool opened = fgets(file_path, 1024, file);
		if (opened)
		{
			std::string path = std::string(file_path);
			path.erase(std::find(path.begin(), path.end(), '\n'), path.end());
			return path;
		}
		pclose(file);
		return std::string();
	}
}

#endif