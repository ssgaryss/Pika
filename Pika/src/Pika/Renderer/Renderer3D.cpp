#include "pkpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"

namespace Pika {

#define MAX_MESHES_PER_BATCH 10000 // for now

	struct LineVertexData
	{
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);

		int m_EntityID = -1;
	};

	struct Renderer3DData
	{
	public:
		// TODO : Change it
		static const uint32_t s_MaxQuadsPerBatch = MAX_MESHES_PER_BATCH;
		static const uint32_t s_MaxVerticesPerBatch = s_MaxQuadsPerBatch * 3;
		static const uint32_t s_MaxIndicesPerBatch = s_MaxQuadsPerBatch * 3;

		// Mesh
		Ref<Shader> m_MeshShader;

		// Line
		Ref<VertexArray> m_LineVertexArray = nullptr;
		Ref<VertexBuffer> m_LineVertexBuffer = nullptr;
		Ref<Shader> m_LineShader = nullptr;
		float m_LineThickness = 0.5f;

		uint32_t m_LineIndexCount = 0;
		LineVertexData* m_pLineVertexBufferBase = nullptr;
		LineVertexData* m_pLineVertexBufferPtr = nullptr;

		// Skybox
		Ref<VertexArray> m_SkyboxVertexArray = nullptr;
		Ref<VertexBuffer> m_SkyboxVertexBuffer = nullptr;
		Ref<Shader> m_SkyboxShader = nullptr;

		struct CameraData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f); // Skybox渲染需要
		};
		CameraData m_CameraData;
		Ref<UniformBuffer> m_CameraDataUniformBuffer;

		Renderer3D::Statistics m_Statistics; // Record the renderer states
	public:
		~Renderer3DData() {
			delete[] m_pLineVertexBufferBase;
		}
	};

	static Renderer3DData s_Data;

	void Renderer3D::Initialize()
	{
		PK_PROFILE_FUNCTION();

		{
			uint32_t Flags = RendererAPI::EnableBlend | RendererAPI::EnableDepthTest
				| RendererAPI::EnableLineSmooth | RendererAPI::EnableCullBackFace;
			RenderCommand::Initialize(Flags);
		}

		s_Data.m_MeshShader = Shader::Create("assets/shaders/Renderer3D/DefaultMeshShader.glsl");

		// Line
		s_Data.m_LineVertexArray = VertexArray::Create();
		s_Data.m_LineVertexArray->bind();
		uint32_t* LineIndicesPerBatch = new uint32_t[Renderer3DData::s_MaxIndicesPerBatch];
		{
			for (uint32_t i = 0; i < Renderer3DData::s_MaxIndicesPerBatch; i++) {
				LineIndicesPerBatch[i] = i;
			}
		}
		Ref<IndexBuffer> LineIndexBuffer = IndexBuffer::Create(LineIndicesPerBatch, Renderer3DData::s_MaxIndicesPerBatch);
		s_Data.m_LineVertexArray->setIndexBuffer(LineIndexBuffer);
		delete[] LineIndicesPerBatch;

		s_Data.m_LineVertexBuffer = VertexBuffer::Create(Renderer3DData::s_MaxVerticesPerBatch * sizeof(LineVertexData));
		BufferLayout LineLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_LineVertexBuffer->setLayout(LineLayout);
		s_Data.m_LineVertexArray->addVertexBuffer(s_Data.m_LineVertexBuffer);
		s_Data.m_LineShader = Shader::Create("assets/shaders/Renderer3D/DefaultLineShader.glsl");

		s_Data.m_pLineVertexBufferBase = new LineVertexData[Renderer3DData::s_MaxVerticesPerBatch];
		s_Data.m_LineVertexArray->unbind();

		// Skybox
		s_Data.m_SkyboxVertexArray = VertexArray::Create();
		s_Data.m_SkyboxVertexArray->bind();
		uint32_t SkyboxIndices[] = {
			0, 1, 2,   2, 3, 0,  // Back face
			4, 5, 6,   6, 7, 4,  // Front face
			4, 5, 1,   1, 0, 4,  // Left face
			3, 2, 6,   6, 7, 3,  // Right face
			4, 0, 3,   3, 7, 4,  // Top face
			1, 5, 6,   6, 2, 1   // Bottom face
		};
		Ref<IndexBuffer> SkyboxIndexBuffer = IndexBuffer::Create(SkyboxIndices, 36);
		s_Data.m_SkyboxVertexArray->setIndexBuffer(SkyboxIndexBuffer);
		float SkyboxVertices[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f, // 8个顶点
		};
		s_Data.m_SkyboxVertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));
		BufferLayout SkyboxLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
		};
		s_Data.m_SkyboxVertexBuffer->setLayout(SkyboxLayout);
		s_Data.m_SkyboxVertexArray->addVertexBuffer(s_Data.m_SkyboxVertexBuffer);
		s_Data.m_SkyboxShader = Shader::Create("assets/shaders/Renderer3D/DefaultSkyboxShader.glsl");
		s_Data.m_SkyboxVertexArray->unbind();

		s_Data.m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_CameraData), 0); // glsl中binding = 1
	}

	void Renderer3D::BeginScene(const EditorCamera& vEditorCamera)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_CameraData.m_ViewProjectionMatrix = vEditorCamera.getViewProjectionMatrix();
		s_Data.m_CameraData.m_ViewMatrix = vEditorCamera.getViewMatrix();
		s_Data.m_CameraData.m_ProjectionMatrix = vEditorCamera.getProjectionMatrix();
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));

		ResetStatistics();
		StartBatch();
	}

	void Renderer3D::BeginScene(const Camera& vCamera, const glm::mat4& vTramsform)
	{
		PK_PROFILE_FUNCTION();
		// TODO !

		ResetStatistics();
		StartBatch();
	}

	void Renderer3D::EndScene()
	{
		PK_PROFILE_FUNCTION();
		Flush();
	}

	void Renderer3D::Flush()
	{
		PK_PROFILE_FUNCTION();

		// Lines
		if (s_Data.m_LineIndexCount) {
			// Set Buffer Data
			ptrdiff_t BatchElementNums = s_Data.m_pLineVertexBufferPtr - s_Data.m_pLineVertexBufferBase;
			uint32_t Size = static_cast<uint32_t>(BatchElementNums) * sizeof(LineVertexData);
			s_Data.m_LineVertexBuffer->setData(s_Data.m_pLineVertexBufferBase, Size);

			s_Data.m_LineShader->bind();
			RenderCommand::SetLineThickness(s_Data.m_LineThickness);
			RenderCommand::DrawLines(s_Data.m_LineVertexArray.get(), s_Data.m_LineIndexCount);
			s_Data.m_Statistics.m_DrawCalls++;
		}
	}

	void Renderer3D::SetLineThickness(float vThickness)
	{
		s_Data.m_LineThickness = vThickness;
	}

	float Renderer3D::GetLineThickness()
	{
		return s_Data.m_LineThickness;
	}

	void Renderer3D::DrawLine(const glm::vec3& vStartPosition, const glm::vec3& vEndPosition, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_LineIndexCount >= Renderer3DData::s_MaxIndicesPerBatch)
			NextBatch();

		s_Data.m_pLineVertexBufferPtr->m_Position = vStartPosition;
		s_Data.m_pLineVertexBufferPtr->m_Color = vColor;
		s_Data.m_pLineVertexBufferPtr->m_EntityID = -1;
		s_Data.m_pLineVertexBufferPtr++;

		s_Data.m_pLineVertexBufferPtr->m_Position = vEndPosition;
		s_Data.m_pLineVertexBufferPtr->m_Color = vColor;
		s_Data.m_pLineVertexBufferPtr->m_EntityID = -1;
		s_Data.m_pLineVertexBufferPtr++;

		s_Data.m_LineIndexCount += 2;
		s_Data.m_Statistics.m_LineCount++;
	}

	void Renderer3D::DrawGrid(const glm::mat4& vIdentityMatrix, float vSize, const glm::vec4& vColor, float vInterval)
	{
		if (vSize < 0.0f || vInterval < 0.0f)
			return;
		uint32_t NumsOfLines = static_cast<uint32_t>(vSize / vInterval);
		glm::vec3 StartPositionHorizontal = { -vSize, -(float)NumsOfLines * vInterval, 0.0f };
		glm::vec3 EndPositionHorizontal = { vSize, -(float)NumsOfLines * vInterval, 0.0f };
		glm::vec3 StartPositionVertical = { -(float)NumsOfLines * vInterval, -vSize, 0.0f };
		glm::vec3 EndPositionVertical = { -(float)NumsOfLines * vInterval, vSize, 0.0f };
		for (uint32_t i = 0; i < NumsOfLines * 2 + 1; ++i) {
			glm::vec3 horizontalOffset = glm::vec3{ 0.0f, vInterval * i, 0.0f };
			glm::vec3 verticalOffset = glm::vec3{ vInterval * i, 0.0f, 0.0f };
			if (vIdentityMatrix != glm::mat4(1.0f)) {
				glm::vec4 TransformedStartHorizontal = vIdentityMatrix * glm::vec4(StartPositionHorizontal + horizontalOffset, 1.0f);
				glm::vec4 TransformedEndHorizontal = vIdentityMatrix * glm::vec4(EndPositionHorizontal + horizontalOffset, 1.0f);

				glm::vec4 TransformedStartVertical = vIdentityMatrix * glm::vec4(StartPositionVertical + verticalOffset, 1.0f);
				glm::vec4 TransformedEndVertical = vIdentityMatrix * glm::vec4(EndPositionVertical + verticalOffset, 1.0f);

				DrawLine(glm::vec3(TransformedStartHorizontal), glm::vec3(TransformedEndHorizontal), vColor);
				DrawLine(glm::vec3(TransformedStartVertical), glm::vec3(TransformedEndVertical), vColor);
			}
			else {
				glm::vec4 TransformedStartHorizontal = glm::vec4(StartPositionHorizontal + horizontalOffset, 1.0f);
				glm::vec4 TransformedEndHorizontal = glm::vec4(EndPositionHorizontal + horizontalOffset, 1.0f);

				glm::vec4 TransformedStartVertical = glm::vec4(StartPositionVertical + verticalOffset, 1.0f);
				glm::vec4 TransformedEndVertical = glm::vec4(EndPositionVertical + verticalOffset, 1.0f);

				DrawLine(glm::vec3(TransformedStartHorizontal), glm::vec3(TransformedEndHorizontal), vColor);
				DrawLine(glm::vec3(TransformedStartVertical), glm::vec3(TransformedEndVertical), vColor);
			}

		}
	}

	void Renderer3D::RenderSkybox(const Ref<Cubemap>& vSkybox)
	{
		if (vSkybox) {
			s_Data.m_SkyboxShader->bind();
			vSkybox->bind(0);
			s_Data.m_SkyboxShader->setInt("u_Skybox", 0);
			RenderCommand::DrawIndexed(s_Data.m_SkyboxVertexArray.get(), 36);
		}
	}

	void Renderer3D::ResetStatistics()
	{
		std::memset(&s_Data.m_Statistics, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStatistics()
	{
		return s_Data.m_Statistics;
	}

	void Renderer3D::StartBatch()
	{
		// Line
		s_Data.m_LineIndexCount = 0;
		s_Data.m_pLineVertexBufferPtr = s_Data.m_pLineVertexBufferBase;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

}