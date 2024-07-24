#pragma once
#include "SceneCamera.h"
#include <string>
#include <glm/glm.hpp>

namespace Pika
{

	struct TagComponent
	{
		std::string m_Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& vTag)
			: m_Tag{ vTag } {}

		operator std::string() const { return m_Tag; }
		operator const char* () const { return m_Tag.c_str(); }
	};

	struct TransformComponent
	{
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& vPosition)
			: m_Position{ vPosition } {}
		TransformComponent(const TransformComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		// TODO : texuture!

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& vColor)
			: m_Color{ vColor } {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera m_Camera;
		bool m_IsFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		// TODO
	};

}