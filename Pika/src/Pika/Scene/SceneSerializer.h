#pragma once
#include "Scene.h"

namespace Pika
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& vScene);

		void serializeYAMLText(const std::string& vFilePath);
		void serializeYAMLBinary(const std::string& vFilePath);
		void deserializeYAMLText(const std::string& vFilePath);
		void deserializeYAMLBinary(const std::string& vFilePath);
	private:
		Ref<Scene> m_Scene;
	};

}
