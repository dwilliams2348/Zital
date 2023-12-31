#pragma once

#include <string>

namespace Zital
{

	class FileDialogs
	{
	public:
		//Returns empty string if dialog is cancelled
		static std::string OpenFile(const char* _filter);
		static std::string SaveFile(const char* _filter);
	};

}