#pragma once
#include <chrono>

namespace Pika {

	class InstrumentorTimer
	{
	public:
		InstrumentorTimer();
		~InstrumentorTimer();

		inline void stop() {

		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	};

}