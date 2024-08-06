#include "pkpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>


// TODO : yaml对于glm数据类型输入流运算符重载，暂时放这
namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, const glm::vec3& vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y << vData.z;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, const glm::vec4& vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y << vData.z << vData.w;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

}


namespace Pika {

	SceneSerializer::SceneSerializer(const Ref<Scene>& vScene)
		: m_Scene{ vScene } {}

	void SceneSerializer::serializeYAMLText(const std::string& vFilePath)
	{
		std::string SceneName = "Untitled"; // TODO : Scene name!
		YAML::Emitter Out;

		Out << YAML::BeginMap; // 每个Scene
		{
			Out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap;
			{
				Out << YAML::Key << "Name" << YAML::Value << SceneName;
				Out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // 所有Entities
				{
					m_Scene->m_Registry.view<IDComponent>().each([&Out, this](auto vEntity, auto& vTagComponent) {
						Entity Entity{ vEntity, m_Scene.get() };
						Out << YAML::BeginMap; // 每个Entity
						if (Entity.hasComponent<IDComponent>()) {
							Out << YAML::Key << "Entity" << YAML::Value << Entity.getComponent<IDComponent>().m_ID;
						}
						if (Entity.hasComponent<TagComponent>()) {
							Out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Tag" << YAML::Value << Entity.getComponent<TagComponent>().m_Tag;
								Out << YAML::EndMap;
							}
						}
						if (Entity.hasComponent<TransformComponent>()) {
							auto& Transform = Entity.getComponent<TransformComponent>();
							Out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Position" << YAML::Value << Transform.m_Position;
								Out << YAML::Key << "Rotation" << YAML::Value << Transform.m_Rotation;
								Out << YAML::Key << "Scale" << YAML::Value << Transform.m_Scale;
								Out << YAML::EndMap;
							}
						}
						if (Entity.hasComponent<SpriteRendererComponent>()) {
							auto& SpriteRenderer = Entity.getComponent<SpriteRendererComponent>();
							Out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Color" << YAML::Value << SpriteRenderer.m_Color;
								Out << YAML::EndMap;
							}
						}
						if (Entity.hasComponent<CameraComponent>()) {
							Out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
							{
								auto& Camera = Entity.getComponent<CameraComponent>();
								Out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
								{
									Out << YAML::Key << "ProjectionMode" << YAML::Value << static_cast<int>(Camera.m_Camera.getProjectionMode());
									Out << YAML::Key << "OthographicSize" << YAML::Value << Camera.m_Camera.getOthographicSize();
									Out << YAML::Key << "OthographicNear" << YAML::Value << Camera.m_Camera.getOthographicNear();
									Out << YAML::Key << "OthographicFar" << YAML::Value << Camera.m_Camera.getOthographicFar();
									Out << YAML::Key << "PerspectiveFOV" << YAML::Value << Camera.m_Camera.getPerspectiveFOV();
									Out << YAML::Key << "PerspectiveNear" << YAML::Value << Camera.m_Camera.getPerspectiveNear();
									Out << YAML::Key << "getPerspectiveFar" << YAML::Value << Camera.m_Camera.getPerspectiveFar();
									Out << YAML::Key << "AspectRatio" << YAML::Value << Camera.m_Camera.getAspectRatio();
								}
								Out << YAML::EndMap;
								Out << YAML::Key << "IsFixedAspectRatio" << YAML::Value << Camera.m_IsFixedAspectRatio;
								Out << YAML::EndMap;
							}
						}

						Out << YAML::EndMap;
						});
				}
				Out << YAML::EndSeq;
			}
			Out << YAML::EndMap;
		}
		Out << YAML::EndMap;

		// Write in file
		std::ofstream FileOut(vFilePath, std::ios_base::out | std::ios_base::trunc);
		PK_CORE_TRACE(R"(SceneSerializer : Try to serialize .pika file at "{0}" ...)", vFilePath);
		if (!FileOut.is_open()) {
			PK_CORE_ERROR(R"(SceneSerializer : Fail to open yaml file at "{0}".)", vFilePath);
			return;
		}
		FileOut << Out.c_str();
		PK_CORE_INFO(R"(SceneSerializer : Success to save scene "{0}".)", SceneName);
	}

	void SceneSerializer::serializeYAMLBinary(const std::string& vFilePath)
	{
		//TODO
	}

	bool SceneSerializer::deserializeYAMLText(const std::string& vFilePath)
	{
		YAML::Node Data;
		try
		{
			Data = YAML::LoadFile(vFilePath);
		}
		catch (const YAML::ParserException& e)
		{
			PK_CORE_ERROR(R"(SceneSerializer : Failed to load .pika file at "{0}".)", vFilePath);
			std::cerr << e.what() << std::endl;
			return false;
		}

		if (!Data["Scene"]) {
			PK_CORE_ERROR(R"(SceneSerializer : Not a valid scene yaml file "{0}")", vFilePath);
			return false;
		}

		PK_CORE_TRACE(R"(SceneSerializer : Try to deserialize .pika file at "{0}" ...)", vFilePath);
		auto SceneNode = Data["Scene"];
		// Name
		std::string SceneName = SceneNode["Name"].as<std::string>(); // TODO!
		// Entities
		YAML::Node Entities = SceneNode["Entities"];
		if (Entities) {
			for (auto Entity : Entities) {
				auto UUIDString = Entity["Entity"].as<std::string>();
				std::string EntityName;
				if (Entity["TagComponent"]) {
					auto TagComponentNode = Entity["TagComponent"];
					EntityName = TagComponentNode["Tag"].as<std::string>();
				}
				auto DeserializedEntity = m_Scene->createEntityWithUUIDString(UUIDString, EntityName);
				PK_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", UUIDString, EntityName);

				if (Entity["TransformComponent"]) {
					auto TransformComponentNode = Entity["TransformComponent"];
					auto& TC = DeserializedEntity.getComponent<TransformComponent>();
					TC.m_Position = TransformComponentNode["Position"].as<glm::vec3>();
					TC.m_Rotation = TransformComponentNode["Rotation"].as<glm::vec3>();
					TC.m_Scale = TransformComponentNode["Scale"].as<glm::vec3>();
				}

				if (Entity["SpriteRendererComponent"]) {
					auto SpriteRendererComponentNode = Entity["SpriteRendererComponent"];
					auto& SRC = DeserializedEntity.addComponent<SpriteRendererComponent>();
					SRC.m_Color = SpriteRendererComponentNode["Color"].as<glm::vec4>();
				}

				if (Entity["CameraComponent"]) {
					auto CameraComponentNode = Entity["CameraComponent"];
					auto& SRC = DeserializedEntity.addComponent<CameraComponent>();
					// TODO!
				}
			}
		}

		PK_CORE_INFO(R"(SceneSerializer : Success to load scene "{0}".)", SceneName);
		return true;
	}

	bool SceneSerializer::deserializeYAMLBinary(const std::string& vFilePath)
	{
		// TODO
		return false;
	}

}