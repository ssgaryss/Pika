#pragma once


namespace Pika {

	class Timestep {
	public:
		Timestep(float vTime = 0.0f)
			: m_Time{ vTime }{}
		operator float() const { return m_Time; }
		float getSeconds() const { return m_Time; }
		float getMiliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};

}