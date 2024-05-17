#include "Log.h"

namespace Pika {
	std::shared_ptr<spdlog::logger> Log::m_pCoreLogger;	// �������־
	std::shared_ptr<spdlog::logger> Log::m_pClientLogger; // �ͻ�����־

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_pCoreLogger = spdlog::stdout_color_mt("Pika");
		m_pCoreLogger->set_level(spdlog::level::level_enum::trace);
		m_pClientLogger = spdlog::stdout_color_mt("APP");
		m_pClientLogger->set_level(spdlog::level::level_enum::trace);
	}
}

