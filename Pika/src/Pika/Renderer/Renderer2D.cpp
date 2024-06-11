#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>


namespace Pika {

	struct Renderer2DData {
		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_QuadShader;

		Ref<Texture> m_WhiteTexture; // for now

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

		// for now TODO: DELETE!
		float vertices_pos[4 * 5]
		{
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,// top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left 
		};
		uint32_t indices[6]{
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle 
		};

		s_Data.m_QuadVertexBuffer = VertexBuffer::Create(vertices_pos, sizeof(vertices_pos));
		Pika::BufferLayout QuadLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"}
		};
		s_Data.m_QuadVertexBuffer->setLayout(QuadLayout);
		s_Data.m_QuadVertexArray->addVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadVertexArray->setIndexBuffer(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		s_Data.m_QuadShader = Shader::Create("assets/shaders/Renderer2D/DefaultQuadShader.glsl");

		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
	}

	void Renderer2D::BeginScene(Camera2DController& vCameraController)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCameraController.getCamera().getViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
		PK_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		drawQuad(glm::vec3(vPosition, 0.0f), vScale, vTintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		s_Data.m_QuadShader->setFloat("u_TilingFactor", 1.0f);
		s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		s_Data.m_WhiteTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		drawQuad(glm::vec3(vPosition, 0.0f), vScale, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
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

	void Renderer2D::drawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		drawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vTintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::rotate(glm::mat4(1.0f), vRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		s_Data.m_QuadShader->setMat4("u_Transform", Transform);
		s_Data.m_QuadShader->setFloat("u_TilingFactor", 1.0f);
		s_Data.m_QuadShader->setFloat4("u_TintColor", vTintColor);
		s_Data.m_WhiteTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
	}

}