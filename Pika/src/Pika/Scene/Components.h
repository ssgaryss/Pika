#pragma once
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
	};

	struct TransformComponent
	{
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		glm::vec4 m_Color{ 1.0f,1.0f,1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& vColor)
			: m_Color{ vColor } {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct CameraComponent
	{

	};

}