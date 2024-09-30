#pragma once
#include "SceneCamera.h"
#include "Model.h"
#include "Materials.h"
#include "Lights.h"
#include "Pika/Core/UUID.h"
#include "Pika/Renderer/Texture.h"
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

	struct CameraComponent
	{
		SceneCamera m_Camera;
		bool m_IsFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// 3D only
	struct ModelComponent
	{
		Ref<Model> m_Model = nullptr;

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default; // 感觉浅拷贝就行，毕竟Mesh数据是复用的
	};

	struct MaterialComponent
	{
		Ref<Material> m_Material = nullptr;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent& vOther) {
			m_Material = vOther.m_Material ? vOther.m_Material->clone() : nullptr; // 这里深拷贝感觉好一点，因为我并没有把Material作为一种Asset，也就没有复用
		}
	};

	struct LightComponent
	{
		Ref<Light> m_Light = nullptr;

		LightComponent() = default;
		LightComponent(const LightComponent& vOther) {
			m_Light = vOther.m_Light ? vOther.m_Light->clone() : nullptr;
		}
	};

	// 2D only
	struct SpriteRendererComponent
	{
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> m_Texture = nullptr;
		glm::vec2 m_TilingFactor = glm::vec2(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& vColor)
			: m_Color{ vColor } {}
		SpriteRendererComponent(const SpriteRendererComponent&) = default; // Texture复用，浅拷贝
	};

	struct Rigidbody2DComponent
	{
		enum class RigidbodyType {
			Static = 0,
			Kinematic,
			Dynamic
		};
		RigidbodyType m_Type = RigidbodyType::Static;
		bool m_IsFixedRotation = false;

		void* m_RuntimeBody = nullptr; // 记录Play模式的b2Body

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

		void* m_RuntimeFixture = nullptr;  // 暂时没用到

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct NativeScriptComponent {
		// TODO
	};

}