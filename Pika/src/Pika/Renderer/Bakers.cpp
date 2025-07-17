#include "pkpch.h"
#include "Bakers.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Framebuffer.h"
#include "RenderCommand.h"

namespace Pika {

	struct IBLBakerData {
	public:
		inline static const std::filesystem::path s_GenerateGGXIntergrationLUTShaderPath = std::filesystem::path("resources/shaders/Baking/GenerateGGXIntergrationLUT.glsl");
	public:
		Ref<Framebuffer> m_BakingFramebuffer = nullptr;

		// Irradiance Map
		Ref<Shader> m_BakeIrradianceMapShader = nullptr;

		// Prefiltered Environment Map
		Ref<Shader> m_BakePrefilteredEnvironmentMapShader = nullptr;

		// BRDF LUT
		Ref<Shader> m_GenerateGGXIntergrationLUTShader = nullptr;
		Ref<VertexArray> m_GenerateGGXIntergrationLUTVertexArray = nullptr;
		Ref<IndexBuffer> m_GenerateGGXIntergrationLUTIndexBuffer = nullptr;
		Ref<VertexBuffer> m_GenerateGGXIntergrationLUTVertexBuffer = nullptr;
	};

	static Ref<IBLBakerData> s_IBLBakerData = nullptr;

	void IBLBaker::Initialize()
	{
		PK_CORE_TRACE("Try to initialize Pika IBL Baker ...");

		s_IBLBakerData.reset();
		s_IBLBakerData = CreateRef<IBLBakerData>();

		s_IBLBakerData->m_BakingFramebuffer = Framebuffer::Create({ 1, 1, 1,{TextureFormat::RGB16F}, false });

		// BRDF LUT
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray = VertexArray::Create();
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->bind();
		constexpr uint32_t QuadIndices[] = {
			0, 1, 2,
			2, 1, 3,
		};
		s_IBLBakerData->m_GenerateGGXIntergrationLUTIndexBuffer = IndexBuffer::Create(QuadIndices, sizeof(QuadIndices) / sizeof(QuadIndices[0]));
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->setIndexBuffer(s_IBLBakerData->m_GenerateGGXIntergrationLUTIndexBuffer);
		constexpr float QuadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  // 0: 左上
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // 1: 左下
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // 2: 右上
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f   // 3: 右下
		};
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexBuffer = VertexBuffer::Create(QuadVertices, sizeof(QuadVertices));
		BufferLayout BRDFLUTBufferLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
		};
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexBuffer->setLayout(BRDFLUTBufferLayout);
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->addVertexBuffer(s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexBuffer);
		s_IBLBakerData->m_GenerateGGXIntergrationLUTShader = Shader::Create(IBLBakerData::s_GenerateGGXIntergrationLUTShaderPath);
		s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->unbind();
		PK_CORE_INFO("IBLBaker : Success to initialize Pika IBL Baker!");
	}

	IBLData IBLBaker::BakeIBLResources(const IBLSpecification& vSpecification)
	{
		PK_PROFILE_FUNCTION();
		PK_CORE_TRACE("IBLBaker : Start to bake the IBL resources.");

		IBLData Data;
		Data.m_BRDFLUT = GenerateBRDFLUT(vSpecification.m_BRDFLUTSpecification);

		PK_CORE_INFO("IBLBaker : Success baking the IBL resources.");
		return Data;
	}

	Ref<Cubemap> IBLBaker::BakeIrradianceMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap)
	{
		// TODO
		return Ref<Cubemap>();
	}

	Ref<Cubemap> IBLBaker::BakePrefilteredEnvironmentMap(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap)
	{
		// TODO
		return Ref<Cubemap>();
	}

	Ref<Texture2D> IBLBaker::GenerateBRDFLUT(const TextureSpecification& vSpecification)
	{
		PK_PROFILE_FUNCTION();

		Ref<Texture2D> BRDFLUT = Texture2D::Create(vSpecification);
		if (s_IBLBakerData) {
			PK_CORE_TRACE("IBLBaker : Start baking BRDF LUT.");
			uint32_t Width = BRDFLUT->getWidth();
			uint32_t Height = BRDFLUT->getHeight();
			s_IBLBakerData->m_BakingFramebuffer->resize(Width, Height);
			s_IBLBakerData->m_BakingFramebuffer->bind();
			s_IBLBakerData->m_BakingFramebuffer->setColorAttachment(0, BRDFLUT);
			s_IBLBakerData->m_BakingFramebuffer->setViewport(0, 0, Width, Height);
			s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->bind();
			s_IBLBakerData->m_GenerateGGXIntergrationLUTShader->bind();
			RenderCommand::DrawIndexed(s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray.get(),
				s_IBLBakerData->m_GenerateGGXIntergrationLUTIndexBuffer->getCount()); // Render LUT
			s_IBLBakerData->m_GenerateGGXIntergrationLUTShader->unbind();
			s_IBLBakerData->m_GenerateGGXIntergrationLUTVertexArray->unbind();
			s_IBLBakerData->m_BakingFramebuffer->unbind();
		}
		else {
			PK_CORE_ERROR("IBLBaker : Attempted to bake BRDF LUT before initialization.");
		}

		return BRDFLUT;
	}

}
