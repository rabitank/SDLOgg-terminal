#pragma once

#include <pch.hpp>
#include <chrono>
#include "SDL.h"

namespace fs 
{

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Timer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		/// @brief 创建 -> 当前调用 经过的时间 /s
		/// @return 
		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

        /// @brief 创建 -> 当前调用 经过的时间 /ms
		/// @return 
		float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}

uint32_t GetDeltaTick();
