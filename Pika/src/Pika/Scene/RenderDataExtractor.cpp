#include "pkpch.h"
#include "RenderDataExtractor.h"

namespace Pika {


	const Ref<Cubemap>& RenderDataExtractor::extractSkybox() const
	{
		return m_Scene->m_Skybox;
	}

}