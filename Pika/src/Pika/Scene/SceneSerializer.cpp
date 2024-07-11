#include "pkpch.h"
#include "SceneSerializer.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>


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
				Out << YAML::BeginMap;
				// TODO : Tag -> UUID
				//m_Scene->m_Registry.view<TagComponent>().each([&Out](auto& vEntity, auto& vTagComponent) {
				//	Out << YAML::Key << "Entity" << YAML::Value << "7897946546163";
				//	if (vEntity.hasCompoent<TagComponent>) {
				//		Out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
				//		{
				//			Out << YAML::Key << "Tag" << YAML::Value << vEntity.getComponent<TagComponent>().m_Tag;
				//			Out << YAML::EndMap;
				//		}
				//	}
				//	});
				Out << YAML::EndMap;
			}
			Out << YAML::EndSeq;
		}
		Out << YAML::EndMap;

		// Write in file
		std::ofstream FileOut(vFilePath, std::ios_base::trunc);
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