#pragma once
#include "SceneCamera.h"
#include "Pika/Core/UUID.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pika
{

	struct IDComponent
	{
		UUID m_ID;

		IDComponent() = default;
		IDComponent(const UUID& vUUID)
			: m_ID{ vUUID } {}
		IDComponent(const IDComponent&) = default;
		operator UUID() const { return m_ID; }
	};

	struct TagComponent
	{
		std::string m_Tag;

		TagComponent() = default;
		TagComponent(const std::string& vTag)
			: m_Tag{ vTag } {}
		TagComponent(const TagComponent&) = default;

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

		operator glm::mat4() const {
			return glm::translate(glm::mat4(1.0f), m_Position) *
				glm::toMat4(glm::quat(glm::radians(m_Rotation))) *
				glm::scale(glm::mat4(1.0f), m_Scale);
		}
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

	struct Rigidbody2DComponent
	{
		enum class RigidbodyType {
			Static = 0,
			Dynamic,
			Kinematic
		};
		RigidbodyType m_Type = RigidbodyType::Static;
		bool m_IsFixedRotation = false;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 m_Offset{ 0.0f,0.0f };
		glm::vec2 m_Size{ 0.5f,0.5f };
		// TODO : Move to physics material
		float m_Density = 1.0f;
		float m_Friction = 0.5f;
		float m_Restitution = 0.0f;
		float m_RestitutionThreshold = 0.5f;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct NativeScriptComponent {
		// TODO
	};

}