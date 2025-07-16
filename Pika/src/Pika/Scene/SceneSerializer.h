#pragma once
#include "Scene.h"
#include <filesystem>

namespace Pika
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& vScene);

		// YAML
		void serializeYAMLText(const std::filesystem::path& vFilePath);
		void serializeYAMLBinary(const std::filesystem::path& vFilePath);
		bool deserializeYAMLText(const std::filesystem::path& vFilePath);
		bool deserializeYAMLBinary(const std::filesystem::path& vFilePath);
	private:
		Ref<Scene> m_Scene;
	};

}
