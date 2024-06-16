#pragma once
#include "Camera2D.h"
#include "Pika/Core/Timestep.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/MouseEvent.h"

namespace Pika
{

	struct Camera2DBounds {
		float m_Left, m_Right;
		float m_Bottom, m_Top;

		inline void setBounds(float vLeft, float vRight, float vBottom, float vTop) {
			m_Left = vLeft;
			m_Right = vRight;
			m_Bottom = vBottom;
			m_Top = vTop;
		}
		inline float getWidth() { return m_Right - m_Left; }
		inline float getHeight() { return m_Top - m_Bottom; }
	};

	class Camera2DController
	{
	public:
		Camera2DController(float vAspectRation, bool vAllowRotation = false);
		void onUpdate(Timestep vTimestep);
		void onEvent(Event& vEvent);

		inline Camera2D getCamera() { return m_Camera; }
		inline const Camera2D getCamera() const { return m_Camera; }

		inline float getZoomLevel() const { return m_ZoomLevel; }
		inline void setZoomLevel(float vZoomLevel) { m_ZoomLevel = vZoomLevel; }

		inline const Camera2DBounds& getBounds() const { return m_Bounds; }
	private:
		bool onWindowResizeEvent(WindowResizeEvent& vEvent);
		bool onMouseScrolledEvent(MouseScrolledEvent& vEvent);
	private:
		float m_AspectRatio = 1.0f; // Width / Height
		float m_ZoomLevel = 1.0f;   // ( top - bottom ) / 2
		bool m_AllowRotation = false;
		Camera2D m_Camera;
		Camera2DBounds m_Bounds;
	};

}
