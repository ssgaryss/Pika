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
		Ref<Texture> m_Texture1; // for now
		Ref<Texture> m_WhiteTexture; // for now

		struct Camera2DData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		};
		Camera2DData m_Camera2DData;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
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
		
		s_Data.m_Texture1 = Texture2D::Create("assets/textures/board.png");
		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
	}

	void Renderer2D::BeginScene(Camera2DController& vCameraController)
	{
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCameraController.getCamera().getViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		s_Data.m_QuadShader->bind();
		s_Data.m_QuadShader->setFloat4("u_TintColor", vColor);
		s_Data.m_WhiteTexture->bind();
		s_Data.m_QuadShader->setInt("u_Texture", 0);
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get());
	}

}