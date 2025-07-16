#pragma once
#include "Texture.h"

namespace Pika {

	struct IBLData {
		Ref<Texture2D> m_IrradianceMap = nullptr;
		Ref<Texture2D> m_PrefilteredEnvironmentMap = nullptr;
		Ref<Texture2D> m_BRDFLUT = nullptr;
	};

	class IBLBaker
	{
	public:
		struct IBLSpecification {
			Ref<Cubemap> m_EnvironmentMap = nullptr;
			TextureSpecification m_IrradianceMapSpecification;
			TextureSpecification m_PrefilteredEnvironmentMapSpecification;
			TextureSpecification m_BRDFLUTSpecification;
		};
	public:
		IBLBaker() = delete;
		static void Initialize();

		static IBLData BakeIBLResources(const IBLSpecification& vSpecification);

	private:
		static Ref<Cubemap> BakeIrradianceMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap);
		static Ref<Cubemap> BakePrefilteredEnvironmentMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap);
		static Ref<Texture2D> GenerateBRDFLUT(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap);
	};

}

