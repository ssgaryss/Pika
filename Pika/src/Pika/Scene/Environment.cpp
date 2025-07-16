#include "pkpch.h"
#include "Environment.h"

namespace Pika {

	Environment::Environment(const std::filesystem::path& vPath)
	{
		m_Skybox = Cubemap::Create(vPath);
	}

	Environment::Environment(const Ref<Cubemap>& vSkybox)
		: m_Skybox{ vSkybox }
	{
		refresh();
	}

	void Environment::refresh()
	{
		if (!m_Skybox) {
			IBLBaker::IBLSpecification Specification;
			Specification.m_EnvironmentMap = m_Skybox;
			m_IBLData = bakingIBLData(Specification);
		}
	}

	Ref<IBLData> Environment::bakingIBLData(const IBLBaker::IBLSpecification& vSpecification)
	{
		return CreateRef<IBLData>(IBLBaker::BakeIBLResources(vSpecification));
	}

}
