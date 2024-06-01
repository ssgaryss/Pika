#pragma once

#include <chrono>

namespace Pika {

	class Timer {
	public:
		Timer() { reset(); }
		inline void reset() { m_StartTime = std::chrono::high_resolution_clock::now(); }
		inline float elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count() * 0.001f * 0.001f * 0.001f;
		}
		inline float elapsedMilis() { return elapsed() * 1000.0f; }
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	};

}
