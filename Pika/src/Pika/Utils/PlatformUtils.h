#pragma once
#include <string>

namespace Pika {

	class FileDialogs 
	{
	public:
		static std::string OpenFile(const char* vFilter);
		static std::string SaveFile(const char* vFilter);
	};

}