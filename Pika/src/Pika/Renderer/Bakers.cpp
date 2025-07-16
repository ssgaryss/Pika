#include "pkpch.h"
#include "Bakers.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

namespace Pika {

	struct IBLBakerData {
	public:
		inline static const std::filesystem::path s_GenerateGGXIntergrationLUTShaderPath = std::filesystem::path("resources/shaders/Baking/GenerateGGXIntergrationLUTShader.glsl");
	public:
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

	static IBLBakerData s_IBLBakerData;

	void IBLBaker::Initialize()
	{
		PK_CORE_TRACE("Try to initialize Pika IBL Baker ...");

		// BRDF LUT
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexArray = VertexArray::Create();
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexArray->bind();
		constexpr uint32_t QuadIndices[] = {
			0, 1, 2,
			2, 1, 3,
		};
		s_IBLBakerData.m_GenerateGGXIntergrationLUTIndexBuffer = IndexBuffer::Create(QuadIndices, sizeof(QuadIndices) / sizeof(QuadIndices[0]));
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexArray->setIndexBuffer(s_IBLBakerData.m_GenerateGGXIntergrationLUTIndexBuffer);
		constexpr float QuadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexBuffer = VertexBuffer::Create(QuadVertices, sizeof(QuadVertices));
		BufferLayout BRDFLUTBufferLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
		};
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexBuffer->setLayout(BRDFLUTBufferLayout);
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexArray->addVertexBuffer(s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexBuffer);
		s_IBLBakerData.m_GenerateGGXIntergrationLUTShader = Shader::Create(IBLBakerData::s_GenerateGGXIntergrationLUTShaderPath);
		s_IBLBakerData.m_GenerateGGXIntergrationLUTVertexArray->unbind();
		PK_CORE_INFO("Success to initialize Pika IBL Baker!");
	}

	IBLData IBLBaker::BakeIBLResources(const IBLSpecification& vSpecification)
	{
		IBLData Data;
		Data.m_BRDFLUT = GenerateBRDFLUT(vSpecification.m_BRDFLUTSpecification, vSpecification.m_EnvironmentMap);
		return Data;
	}

	Ref<Texture2D> IBLBaker::GenerateBRDFLUT(const TextureSpecification& vSpecification, const Ref<Cubemap>& vEnvironmentMap)
	{
		Ref<Texture2D> BRDFLUT = Texture2D::Create(vSpecification);

		return BRDFLUT;
	}

}
