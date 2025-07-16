#pragma once
#include "Pika/Renderer/Texture.h"
#include "Pika/Renderer/Bakers.h"
#include <filesystem>

namespace Pika {

	class Environment
	{
	public:
		Environment() = default;
		Environment(const std::filesystem::path& vPath);
		Environment(const Ref<Cubemap>& vSkybox);

		void refresh(); // 刷新状态，重新烘焙

		inline const Ref<Cubemap>& getSkybox() const { return m_Skybox; }
		inline void setSkybox(const Ref<Cubemap>& vSkybox) { m_Skybox = vSkybox; }
		inline const Ref<IBLData>& getIBLData() const { return m_IBLData; }
		inline void setIBLData(const Ref<IBLData>& vIBLData) { m_IBLData = vIBLData; }
	private:
		Ref<IBLData> bakingIBLData(const IBLBaker::IBLSpecification& vSpecification);
	private:
		Ref<Cubemap> m_Skybox = nullptr;
		Ref<IBLData> m_IBLData = nullptr;
	};

}

