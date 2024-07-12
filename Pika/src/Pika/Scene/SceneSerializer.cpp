#include "pkpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>

// TODO : yaml对于glm数据类型输入流运算符重载，暂时放这
namespace YAML {

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, glm::vec3 vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y << vData.z;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, glm::vec4 vData) {
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
		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "Scene" << YAML::Value << "Untitled"; // TODO : Scene name!
		{
			Out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
			{
				// TODO : Tag -> UUID
				m_Scene->m_Registry.view<TagComponent>().each([&Out, this](auto vEntity, auto& vTagComponent) {
					Entity Entity{ vEntity, m_Scene.get() };
					Out << YAML::BeginMap;
					Out << YAML::Key << "Entity" << YAML::Value << "7897946546163";
					if (Entity.hasComponent<TagComponent>()) {
						Out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
						{
							Out << YAML::Key << "Tag" << YAML::Value << Entity.getComponent<TagComponent>().m_Tag;
							Out << YAML::EndMap;
						}
					}
					if (Entity.hasComponent<TransformComponent>()) {
						Out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
						{
							Out << YAML::Key << "Position" << YAML::Value << Entity.getComponent<TransformComponent>().m_Position;
							Out << YAML::Key << "Rotation" << YAML::Value << Entity.getComponent<TransformComponent>().m_Rotation;
							Out << YAML::Key << "Scale" << YAML::Value << Entity.getComponent<TransformComponent>().m_Scale;
							Out << YAML::EndMap;
						}
					}
					if (Entity.hasComponent<SpriteRendererComponent>()) {
						Out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
						{
							Out << YAML::Key << "Color" << YAML::Value << Entity.getComponent<SpriteRendererComponent>().m_Color;
							Out << YAML::EndMap;
						}
					}
					Out << YAML::EndMap;
					});
				//auto View = m_Scene->m_Registry.view<TagComponent>();
			}
			Out << YAML::EndSeq;
		}
		Out << YAML::EndMap;

		// Write in file
		std::ofstream FileOut(vFilePath, std::ios_base::out | std::ios_base::trunc);
		if (!FileOut.is_open()) {
			PK_CORE_ERROR(R"(SceneSerilizer : Fail to open yaml file "{0}")", vFilePath);
			return;
		}
		FileOut << Out.c_str();
	}

	void SceneSerializer::serializeYAMLBinary(const std::string& vFilePath)
	{
		//TODO
	}

	bool SceneSerializer::deserializeYAMLText(const std::string& vFilePath)
	{
		return false;
	}

	bool SceneSerializer::deserializeYAMLBinary(const std::string& vFilePath)
	{
		// TODO
		return false;
	}

}