
#include "pch.hpp"
#include "core/log.hpp"


namespace fs
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

 	void Log::Init()
	{

		s_CoreLogger = spdlog::stdout_color_mt("FS");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->set_pattern("%^[%T] %n: %v%$");
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_pattern("%^[%T] %n: %v%$");
	//you can get help at https://spdlog.docsforge.com/v1.x/4.sinks/#available-sinks
	}
}



// 区分此函数是在Windows环境调用还是Linux环境调用
#if defined (_WIN64) || defined (WIN32) || defined (_WIN32)

//printf("---Windows---\n");
#include <direct.h>

#else

//printf("---Linux---\n");
#include <unistd.h>

#endif

std::string getOperationFilePath() {

	char *buffer = NULL;

	// 区分此函数是在Windows环境调用还是Linux环境调用
#if defined (_WIN64) || defined (WIN32) || defined (_WIN32)

	// 获取项目的工作路径
	buffer = _getcwd(NULL, 0);

#else

	// 获取项目的工作路径
	buffer = getcwd(NULL, 0);

#endif

	if (buffer) {
		std::string path = buffer;
		free(buffer);
		return path ;
	}

	return "";
}
