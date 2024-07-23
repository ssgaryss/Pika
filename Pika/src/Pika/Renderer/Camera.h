#pragma once
#include <glm/glm.hpp>

namespace Pika {

	// Only envolve camera intrinsic parameters
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& vProjectionMatrix)
			: m_ProjectionMatrix{ vProjectionMatrix } {}
		Camera(const Camera&) = default;
		virtual ~Camera() = default;

		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
	protected:
		enum class CameraProjectionMode {
			None = 0,
			Othographic = 1,
			Perspective = 2
		};
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

}