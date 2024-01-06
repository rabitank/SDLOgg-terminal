#pragma once

// pure hpp included to base.hpp

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/fmt/ostr.h" 

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"



#pragma warning(pop)

namespace fs
{
	class Log //spdlog 包装器
	{
	public:

		static void Init();
		inline static   std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		 static std::shared_ptr<spdlog::logger> s_CoreLogger;
		 static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

	template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
	{
		return os << glm::to_string(vector);
	}

	template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
	{
		return os << glm::to_string(matrix);
	}

	template<typename OStream, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
	{
		return os << glm::to_string(quaternion);
	}
}
//core macro

/// @brief 得到当前文件的工作路径
/// @return 
std::string getOperationFilePath();


#define FS_CORE_TRACE(...)	::fs::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FS_CORE_INFO(...)	::fs::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FS_CORE_WARN(...)	::fs::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FS_CORE_ERROR(...)	::fs::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FS_CORE_FATAL(...)	::fs::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define FS_CORE_CRITICAL(...)	::fs::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define FS_TRACE(...)	::fs::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FS_INFO(...)	::fs::Log::GetClientLogger()->info(__VA_ARGS__)
#define FS_WARN(...)	::fs::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FS_ERROR(...)	::fs::Log::GetClientLogger()->error(__VA_ARGS__)
#define FS_FATAL(...)	::fs::Log::GetClientLogger()->fatal(__VA_ARGS__)

#define FS_ENABLE_ASSERTS
