#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>


namespace Pika {

#define MAX_QUADS_PER_BATCH 1000 // for now

	struct QuadVertexData {
		glm::vec3 m_Position;
		glm::vec4 m_Color;
		glm::vec2 m_TexCoord;
		float m_TextureIndex = 0.0f;
		float m_TilingFactor = 1.0f;
	};

	struct Renderer2DData {
		static const uint32_t s_MaxQuadsPerBatch = MAX_QUADS_PER_BATCH;
		static const uint32_t s_MaxVerticesPerBatch = s_MaxQuadsPerBatch * 4;
		static const uint32_t s_MaxIndicesPerBatch = s_MaxQuadsPerBatch * 6;

		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_QuadShader;
		glm::vec4 m_QuadUnitVertex[4];

		QuadVertexData* m_pQuadVertexBufferBase = nullptr;
		QuadVertexData* m_pQuadVertexBufferPtr = nullptr;

		//TODO : I suppose there are no more than m_MaxTextureSlots textures for now
		static const uint32_t m_MaxTextureSlots = 32;
		std::array<Ref<Texture2D>, m_MaxTextureSlots> m_TextureSlots;
		Ref<Texture2D> m_WhiteTexture; // Default at texture slot 0
		uint32_t m_TextureIndex = 1;

		struct Camera2DData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		};
		Camera2DData m_Camera2DData;

	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		PK_PROFILE_FUNCTION();

		RenderCommand::Init();
		s_Data.m_QuadVertexArray = VertexArray::Create();
		s_Data.m_QuadUnitVertex[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		uint32_t* QuadIndicesPerBatch = new uint32_t[s_Data.s_MaxIndicesPerBatch];
		{
			uint32_t Offset = 0;
			for (uint32_t i = 0; i < s_Data.s_MaxIndicesPerBatch; i += 6) {
				QuadIndicesPerBatch[i + 0] = Offset + 0;
				QuadIndicesPerBatch[i + 1] = Offset + 1;
				QuadIndicesPerBatch[i + 2] = Offset + 2;
				QuadIndicesPerBatch[i + 3] = Offset + 2;
				QuadIndicesPerBatch[i + 4] = Offset + 3;
				QuadIndicesPerBatch[i + 5] = Offset + 0;
				Offset += 4;
			}
		}
		Ref<IndexBuffer> pIndexBuffer = IndexBuffer::Create(QuadIndicesPerBatch, s_Data.s_MaxIndicesPerBatch);
		s_Data.m_QuadVertexArray->setIndexBuffer(pIndexBuffer);
		delete[] QuadIndicesPerBatch;

		s_Data.m_QuadVertexBuffer = VertexBuffer::Create(s_Data.s_MaxVerticesPerBatch * sizeof(QuadVertexData));
		Pika::BufferLayout QuadLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
			{Pika::ShaderDataType::Float, "a_TextureIndex"},
			{Pika::ShaderDataType::Float, "a_TilingFactor"},
		};
		s_Data.m_QuadVertexBuffer->setLayout(QuadLayout);
		s_Data.m_QuadVertexArray->addVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadShader = Shader::Create("assets/shaders/Renderer2D/DefaultQuadShader.glsl");

		s_Data.m_pQuadVertexBufferBase = new QuadVertexData[s_Data.s_MaxVerticesPerBatch];
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		// TODO : This should be used
		//s_Data.m_MaxTextureSlots = RenderCommand::getAvailableTextureSlots();
		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;
		//s_Data.m_TextureSlots.emplace_back(s_Data.m_WhiteTexture); //Ä¬ÈÏSlot0ÊÇ´¿°×ÎÆÀí
		//s_Data.m_TextureSlots.reserve(s_Data.m_MaxTextureSlots);
	}

	void Renderer2D::BeginScene(Camera2DController& vCameraController)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCameraController.getCamera().getViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
		PK_PROFILE_FUNCTION();
		Flush();
	}

	void Renderer2D::Flush()
	{
		PK_PROFILE_FUNCTION();
		// Set Buffer Data
		ptrdiff_t BatchElementNums = s_Data.m_pQuadVertexBufferPtr - s_Data.m_pQuadVertexBufferBase;
		uint32_t Size = static_cast<uint32_t>(BatchElementNums) * sizeof(QuadVertexData);
		s_Data.m_QuadVertexBuffer->setData(s_Data.m_pQuadVertexBufferBase, Size);
		// Bind all textures to slots
		for (uint32_t i = 0; i < s_Data.m_TextureIndex; ++i)
			s_Data.m_TextureSlots[i]->bind(i);
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[s_Data.m_MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.m_MaxTextureSlots; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);

		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

	}

	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		ptrdiff_t BatchElementNums = s_Data.m_pQuadVertexBufferPtr - s_Data.m_pQuadVertexBufferBase;
		if (BatchElementNums >= s_Data.s_MaxVerticesPerBatch) {
			Flush();
		}
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = Transform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vColor;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = 0;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = 1.0f;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		//s_Data.m_QuadShader->bind();
		//s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
		//	glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		//s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		//s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		//s_Data.m_WhiteTexture->bind();
		//s_Data.m_QuadShader->setInt("u_Texture", 0);
		//RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		ptrdiff_t BatchElementNums = s_Data.m_pQuadVertexBufferPtr - s_Data.m_pQuadVertexBufferBase;
		if (BatchElementNums >= s_Data.s_MaxVerticesPerBatch) {
			Flush();
		}
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		float TextureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.m_TextureIndex; ++i) {
			if (*vTexture.get() == *s_Data.m_TextureSlots[i].get()) {
				TextureIndex = static_cast<float>(i);
				break;
			}
		}
		if(TextureIndex == 0.0f){
			TextureIndex = static_cast<float>(s_Data.m_TextureIndex);
			s_Data.m_TextureSlots[s_Data.m_TextureIndex] = vTexture;
			s_Data.m_TextureIndex++;
		}
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = Transform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vTintColor;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = TextureIndex;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = vTilingFactor;
			s_Data.m_pQuadVertexBufferPtr++;
		}
		//s_Data.m_QuadShader->bind();
		//s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
		//	glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		//s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		//s_Data.m_QuadShader->setFloat("u_TilingFactor", vTilingFactor);
		//s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		//vTexture->bind();
		//s_Data.m_QuadShader->setInt("u_Texture", 0);
		//RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor)
	{
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		//s_Data.m_QuadShader->bind();
		//s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
		//	glm::rotate(glm::mat4(1.0f), vRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		//	glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		//s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		//s_Data.m_QuadShader->setFloat("u_TilingFactor", 1.0f);
		//s_Data.m_QuadShader->setFloat4("u_TintColor", vColor);
		//s_Data.m_WhiteTexture->bind();
		//s_Data.m_QuadShader->setInt("u_Texture", 0);
		//RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		//s_Data.m_QuadShader->bind();
		//s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		//glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
		//	glm::rotate(glm::mat4(1.0f), vRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		//	glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		//s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		//s_Data.m_QuadShader->setFloat("u_TilingFactor", vTilingFactor);
		//s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		//vTexture->bind();
		//s_Data.m_QuadShader->setInt("u_Texture", 0);
		//RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

}