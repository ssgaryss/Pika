#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>


namespace Pika {

#define MAX_QUADS_PER_BATCH 10000 // for now

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
		uint32_t s_MaxTextureSlots;

		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_QuadShader;
		glm::vec4 m_QuadUnitVertex[4];

		QuadVertexData* m_pQuadBufferBase = nullptr;
		QuadVertexData* m_pQuadBufferPtr = nullptr;


		Ref<Texture> m_WhiteTexture; // Default at texture slot 0
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
		s_Data.s_MaxTextureSlots = RenderCommand::getAvailableTextureSlots();
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
		s_Data.m_QuadVertexArray->setIndexBuffer(pIndexBuffer);
		s_Data.m_QuadShader = Shader::Create("assets/shaders/Renderer2D/DefaultQuadShader.glsl");

		s_Data.m_pQuadBufferBase = new QuadVertexData[s_Data.s_MaxVerticesPerBatch];
		s_Data.m_pQuadBufferPtr = s_Data.m_pQuadBufferBase;

		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data)); //Ä¬ÈÏSlot0ÊÇ´¿°×ÎÆÀí
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
		s_Data.m_QuadVertexBuffer->setData(s_Data.m_pQuadBufferBase, s_Data.s_MaxVerticesPerBatch * sizeof(QuadVertexData));
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
		s_Data.m_pQuadBufferPtr = s_Data.m_pQuadBufferBase;

	}

	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		ptrdiff_t BatchElementNums = s_Data.m_pQuadBufferPtr - s_Data.m_pQuadBufferBase;
		if (BatchElementNums >= s_Data.s_MaxVerticesPerBatch) {
			Flush();
		}
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadBufferPtr->m_Position = Transform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadBufferPtr->m_Color = vColor;
			s_Data.m_pQuadBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadBufferPtr->m_TextureIndex = 0;
			s_Data.m_pQuadBufferPtr->m_TilingFactor = 1.0f;
			s_Data.m_pQuadBufferPtr++;
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
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		s_Data.m_QuadShader->setFloat("u_TilingFactor", vTilingFactor);
		s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		vTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor)
	{
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::rotate(glm::mat4(1.0f), vRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		s_Data.m_QuadShader->setFloat("u_TilingFactor", 1.0f);
		s_Data.m_QuadShader->setFloat4("u_TintColor", vColor);
		s_Data.m_WhiteTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::rotate(glm::mat4(1.0f), vRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		s_Data.m_QuadShader->setFloat("u_TilingFactor", vTilingFactor);
		s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		vTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

}