#pragma once
#include "Camera2D.h"
#include "Pika/Core/Timestep.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/KeyboardEvent.h"
#include "Pika/Events/MouseEvent.h"

namespace Pika
{

	class Camera2DController
	{
	public:
		Camera2DController(float vAspectRation, bool vAllowRotation = false)
			: m_AspectRatio{ vAspectRation }, m_AllowRotation{ vAllowRotation } {}
		void onUpdate(Timestep vTimestep);
		void onEvent(Event& vEvent);

		inline Camera2D getCamera() { return m_Camera; }
		inline const Camera2D getCamera() const { return m_Camera; }

		inline float getZoomLevel() const { return m_ZoomLevel; }
		inline void setZoomLevel(float vZoomLevel) { m_ZoomLevel = vZoomLevel; }
	private:
		void onWindowResizeEvent(WindowResizeEvent& vEvent);
		void onMouseScrolledEvent(MouseScrolledEvent& vEvent);
	private:
		float m_AspectRatio = 1.0f;
		float m_ZoomLevel = 1.0f;   // ( Right - Left ) / 2
		bool m_AllowRotation = false;
		Camera2D m_Camera;
	};

}
