#pragma once
#include "Scene.h"

namespace Pika
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& vScene);

		// YAML
		void serializeYAMLText(const std::string& vFilePath);
		void serializeYAMLBinary(const std::string& vFilePath);
		bool deserializeYAMLText(const std::string& vFilePath);
		bool deserializeYAMLBinary(const std::string& vFilePath);
	private:
		Ref<Scene> m_Scene;
	};

}
