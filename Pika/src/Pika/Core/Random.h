#pragma once
#include <random>

namespace Pika {

	class Random
	{
	public:
		Random() = delete;
		inline static void Initialize() {
			s_RandomEngine.seed(std::random_device()());
		}
		inline static float GetFloat() {
			return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}
