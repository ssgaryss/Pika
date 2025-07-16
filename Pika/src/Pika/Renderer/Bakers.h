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

			IBLSpecification() {
				// Irradiance
				m_IrradianceMapSpecification.m_Width = 32;
				m_IrradianceMapSpecification.m_Height = 32;
				m_IrradianceMapSpecification.m_Format = TextureFormat::RGB16F;
				m_IrradianceMapSpecification.m_RequiredMips = false;

				// Prefiltered Environment
				m_PrefilteredEnvironmentMapSpecification.m_Width = 128;
				m_PrefilteredEnvironmentMapSpecification.m_Height = 128;
				m_PrefilteredEnvironmentMapSpecification.m_Format = TextureFormat::RGB16F;
				m_PrefilteredEnvironmentMapSpecification.m_RequiredMips = true;

				// BRDF LUT
				m_BRDFLUTSpecification.m_Width = 512;
				m_BRDFLUTSpecification.m_Height = 512;
				m_BRDFLUTSpecification.m_Format = TextureFormat::RG16F;
				m_BRDFLUTSpecification.m_RequiredMips = false;
			}
		};
	public:
		IBLBaker() = delete;
		static void Initialize();

		static IBLData BakeIBLResources(const IBLSpecification& vSpecification);

	private:
		static Ref<Cubemap> BakeIrradianceMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap);
		static Ref<Cubemap> BakePrefilteredEnvironmentMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap);
		static Ref<Texture2D> GenerateBRDFLUT(const TextureSpecification& vSpecification);
	};

}

