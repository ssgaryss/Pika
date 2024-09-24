#include "pkpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderBatch.h"
#include "Pika/Scene/Primitive.h"

namespace Pika {

#define MAX_TRIANGLES_PER_BATCH 10000 // for now
#define MAX_LINES_PER_BATCH 10000 // for now

	struct Renderer3DData
	{
	public:
		static const uint32_t s_MaxTrianglesPerBatch = MAX_TRIANGLES_PER_BATCH;
		static const uint32_t s_MaxTriangleVerticesPerBatch = s_MaxTrianglesPerBatch * 3; // 此处是为了先创建Buffer提高性能
		// StaticMesh
		Ref<VertexArray> m_StaticMeshVertexArray = nullptr;
		Ref<VertexBuffer> m_StaticMeshVertexBuffer = nullptr;
		Ref<Shader> m_StaticMeshShader = nullptr;
		Ref<Shader> m_BlinnPhoneShader = nullptr;
		Ref<UniformBuffer> m_BlinnPhoneMaterialData = nullptr;

		// Line
		static const uint32_t s_MaxLinesPerBatch = MAX_LINES_PER_BATCH;
		static const uint32_t s_MaxLineVerticesPerBatch = s_MaxLinesPerBatch * 2;
		static const uint32_t s_MaxLineIndicesPerBatch = s_MaxLinesPerBatch * 2;
		Ref<VertexArray> m_LineVertexArray = nullptr;
		Ref<VertexBuffer> m_LineVertexBuffer = nullptr;
		Ref<Shader> m_LineShader = nullptr;
		float m_LineThickness = 0.5f;

		uint32_t m_LineIndexCount = 0;
		Ref<RenderBatch<LineVertexData>> m_LineVertexDataBatch = nullptr;

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
	};

	static Renderer3DData s_Data;

	void Renderer3D::Initialize()
	{
		PK_PROFILE_FUNCTION();
		PK_CORE_INFO("Try to initialize Pika 3D Renderer ...");

		{
			uint32_t Flags = RendererAPI::EnableBlend | RendererAPI::EnableDepthTest
				| RendererAPI::EnableLineSmooth | RendererAPI::EnableCullBackFace;
			RenderCommand::Initialize(Flags);
		}

		// StaticMesh
		s_Data.m_StaticMeshVertexArray = VertexArray::Create();
		s_Data.m_StaticMeshVertexArray->bind();
		s_Data.m_StaticMeshVertexBuffer = VertexBuffer::Create(Renderer3DData::s_MaxTriangleVerticesPerBatch * sizeof(StaticMeshVertexData));
		BufferLayout StaticMeshLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float3, "a_Normal"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_StaticMeshVertexBuffer->setLayout(StaticMeshLayout);
		s_Data.m_StaticMeshVertexArray->addVertexBuffer(s_Data.m_StaticMeshVertexBuffer);
		s_Data.m_StaticMeshShader = Shader::Create("resources/shaders/Renderer3D/DefaultStaticMeshShader.glsl");
		s_Data.m_BlinnPhoneShader = Shader::Create("resources/shaders/Renderer3D/DefaultBlinnPhoneShader.glsl");
		s_Data.m_StaticMeshVertexArray->unbind();

		// Line
		s_Data.m_LineVertexArray = VertexArray::Create();
		s_Data.m_LineVertexArray->bind();
		uint32_t* LineIndicesPerBatch = new uint32_t[Renderer3DData::s_MaxLineIndicesPerBatch];
		{
			for (uint32_t i = 0; i < Renderer3DData::s_MaxLineIndicesPerBatch; i++) {
				LineIndicesPerBatch[i] = i;
			}
		}
		Ref<IndexBuffer> LineIndexBuffer = IndexBuffer::Create(LineIndicesPerBatch, Renderer3DData::s_MaxLineIndicesPerBatch);
		s_Data.m_LineVertexArray->setIndexBuffer(LineIndexBuffer);
		delete[] LineIndicesPerBatch;

		s_Data.m_LineVertexBuffer = VertexBuffer::Create(Renderer3DData::s_MaxLineVerticesPerBatch * sizeof(LineVertexData));
		BufferLayout LineLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_LineVertexBuffer->setLayout(LineLayout);
		s_Data.m_LineVertexArray->addVertexBuffer(s_Data.m_LineVertexBuffer);
		s_Data.m_LineShader = Shader::Create("resources/shaders/Renderer3D/DefaultLineShader.glsl");

		s_Data.m_LineVertexDataBatch = CreateRef<RenderBatch<LineVertexData>>(Renderer3DData::s_MaxLineVerticesPerBatch);
		s_Data.m_LineVertexArray->unbind();

		// Skybox
		s_Data.m_SkyboxVertexArray = VertexArray::Create();
		s_Data.m_SkyboxVertexArray->bind();
		uint32_t SkyboxIndices[] = {
			0, 1, 2,   2, 3, 0,  // Back face
			4, 6, 5,   6, 4, 7,  // Front face
			4, 5, 1,   1, 0, 4,  // Left face
			3, 2, 6,   6, 7, 3,  // Right face
			4, 0, 3,   3, 7, 4,  // Top face
			1, 5, 6,   6, 2, 1   // Bottom face
		};
		Ref<IndexBuffer> SkyboxIndexBuffer = IndexBuffer::Create(SkyboxIndices, 36);
		s_Data.m_SkyboxVertexArray->setIndexBuffer(SkyboxIndexBuffer);
		float SkyboxVertices[] = {
			-1.0f,  1.0f, -1.0f, //0
			-1.0f, -1.0f, -1.0f, //1
			 1.0f, -1.0f, -1.0f, //2
			 1.0f,  1.0f, -1.0f, //3
			-1.0f,  1.0f,  1.0f, //4
			-1.0f, -1.0f,  1.0f, //5
			 1.0f, -1.0f,  1.0f, //6
			 1.0f,  1.0f,  1.0f, //7 // 8个顶点
		};
		s_Data.m_SkyboxVertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));
		BufferLayout SkyboxLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
		};
		s_Data.m_SkyboxVertexBuffer->setLayout(SkyboxLayout);
		s_Data.m_SkyboxVertexArray->addVertexBuffer(s_Data.m_SkyboxVertexBuffer);
		s_Data.m_SkyboxShader = Shader::Create("resources/shaders/Renderer3D/DefaultSkyboxShader.glsl");
		s_Data.m_SkyboxVertexArray->unbind();

		s_Data.m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_CameraData), 0);     // glsl中binding = 0
		s_Data.m_BlinnPhoneMaterialData = UniformBuffer::Create(sizeof(BlinnPhoneMaterial::Data), 1);

		PK_CORE_INFO("Success to initialize Pika 3D Renderer!");
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
		if (!s_Data.m_LineVertexDataBatch->empty()) {
			// Set Buffer Data
			s_Data.m_LineVertexBuffer->setData(s_Data.m_LineVertexDataBatch->data(), s_Data.m_LineVertexDataBatch->size());

			s_Data.m_LineShader->bind();
			RenderCommand::SetLineThickness(s_Data.m_LineThickness);
			RenderCommand::DrawLines(s_Data.m_LineVertexArray.get(), s_Data.m_LineIndexCount);
			s_Data.m_Statistics.m_DrawCalls++;
		}
	}

	void Renderer3D::DrawStaticMesh(const glm::mat4& vTransform, const StaticMesh& vMesh, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		Ref<IndexBuffer> StaticMeshIndexBuffer = IndexBuffer::Create(vMesh.getIndicesData(), vMesh.getIndicesCount());
		s_Data.m_StaticMeshVertexArray->setIndexBuffer(StaticMeshIndexBuffer);
		auto TransformVertices = vMesh.getVertices();
		for (auto& Vertex : TransformVertices) {
			Vertex.m_Position = vTransform * glm::vec4(Vertex.m_Position, 1.0f);
			Vertex.m_Normal = vTransform * glm::vec4(Vertex.m_Normal, 1.0f);
			Vertex.m_EntityID = vEntityID;
		}
		s_Data.m_StaticMeshVertexBuffer->setData(TransformVertices.data(), vMesh.getVerticesSize());

		s_Data.m_StaticMeshShader->bind();
		RenderCommand::DrawIndexed(s_Data.m_StaticMeshVertexArray.get(), StaticMeshIndexBuffer->getCount());
		s_Data.m_Statistics.m_MeshCount++;
	}

	void Renderer3D::DrawStaticMesh(const glm::mat4& vTransform, const StaticMesh& vMesh, const MaterialComponent& vMaterial, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		Ref<IndexBuffer> StaticMeshIndexBuffer = IndexBuffer::Create(vMesh.getIndicesData(), vMesh.getIndicesCount());
		s_Data.m_StaticMeshVertexArray->setIndexBuffer(StaticMeshIndexBuffer);
		auto TransformVertices = vMesh.getVertices();
		for (auto& Vertex : TransformVertices) {
			Vertex.m_Position = vTransform * glm::vec4(Vertex.m_Position, 1.0f);
			Vertex.m_Normal = vTransform * glm::vec4(Vertex.m_Normal, 1.0f);
			Vertex.m_EntityID = vEntityID;
		}
		s_Data.m_StaticMeshVertexBuffer->setData(TransformVertices.data(), vMesh.getVerticesSize());

		if(auto BlinnPhone = dynamic_cast<BlinnPhoneMaterial*>(vMaterial.m_Material.get())){
			s_Data.m_BlinnPhoneShader->bind();
			const auto& MaterialData = BlinnPhone->getData();
			s_Data.m_BlinnPhoneMaterialData->setData(&MaterialData, sizeof(MaterialData));
		}
		RenderCommand::DrawIndexed(s_Data.m_StaticMeshVertexArray.get(), StaticMeshIndexBuffer->getCount());
		s_Data.m_Statistics.m_MeshCount++;
	}

	void Renderer3D::DrawModel(const glm::mat4& vTransform, const ModelComponent& vModel, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		for (auto& Mesh : vModel.m_Model->getMeshes()) {
			DrawStaticMesh(vTransform, Mesh, vEntityID);
		}
	}

	void Renderer3D::DrawModel(const glm::mat4& vTransform, const ModelComponent& vModel, const MaterialComponent& vMaterial, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		for (auto& Mesh : vModel.m_Model->getMeshes()) {
			DrawStaticMesh(vTransform, Mesh, vMaterial, vEntityID);
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
		try {
			if (s_Data.m_LineIndexCount >= Renderer3DData::s_MaxLineIndicesPerBatch)
				NextBatch();
			s_Data.m_LineVertexDataBatch->add({ vStartPosition, vColor, -1 });
			s_Data.m_LineVertexDataBatch->add({ vEndPosition, vColor, -1 });
			s_Data.m_LineIndexCount += 2;
		}
		catch (const std::runtime_error& e) {
			PK_CORE_WARN("RenderBatch : {}", e.what());
		}
		s_Data.m_Statistics.m_LineCount++;
	}

	void Renderer3D::DrawGrid(const glm::mat4& vIdentityMatrix, float vSize, const glm::vec4& vColor, float vInterval)
	{
		if (vSize < 0.0f || vInterval < 0.0f)
			return;
		uint32_t NumsOfLines = static_cast<uint32_t>(vSize / vInterval);
		glm::vec3 StartPositionHorizontal = { -vSize, 0.0f, -(float)NumsOfLines * vInterval };
		glm::vec3 EndPositionHorizontal = { vSize, 0.0f, -(float)NumsOfLines * vInterval };
		glm::vec3 StartPositionVertical = { -(float)NumsOfLines * vInterval, 0.0f, -vSize };
		glm::vec3 EndPositionVertical = { -(float)NumsOfLines * vInterval, 0.0f, vSize };
		for (uint32_t i = 0; i < NumsOfLines * 2 + 1; ++i) {
			glm::vec3 HorizontalOffset = glm::vec3{ 0.0f, 0.0f,vInterval * i };
			glm::vec3 VerticalOffset = glm::vec3{ vInterval * i, 0.0f, 0.0f };
			if (vIdentityMatrix != glm::mat4(1.0f)) {
				glm::vec4 TransformedStartHorizontal = vIdentityMatrix * glm::vec4(StartPositionHorizontal + HorizontalOffset, 1.0f);
				glm::vec4 TransformedEndHorizontal = vIdentityMatrix * glm::vec4(EndPositionHorizontal + HorizontalOffset, 1.0f);

				glm::vec4 TransformedStartVertical = vIdentityMatrix * glm::vec4(StartPositionVertical + VerticalOffset, 1.0f);
				glm::vec4 TransformedEndVertical = vIdentityMatrix * glm::vec4(EndPositionVertical + VerticalOffset, 1.0f);

				DrawLine(glm::vec3(TransformedStartHorizontal), glm::vec3(TransformedEndHorizontal), vColor);
				DrawLine(glm::vec3(TransformedStartVertical), glm::vec3(TransformedEndVertical), vColor);
			}
			else {
				glm::vec4 TransformedStartHorizontal = glm::vec4(StartPositionHorizontal + HorizontalOffset, 1.0f);
				glm::vec4 TransformedEndHorizontal = glm::vec4(EndPositionHorizontal + HorizontalOffset, 1.0f);

				glm::vec4 TransformedStartVertical = glm::vec4(StartPositionVertical + VerticalOffset, 1.0f);
				glm::vec4 TransformedEndVertical = glm::vec4(EndPositionVertical + VerticalOffset, 1.0f);

				DrawLine(glm::vec3(TransformedStartHorizontal), glm::vec3(TransformedEndHorizontal), vColor);
				DrawLine(glm::vec3(TransformedStartVertical), glm::vec3(TransformedEndVertical), vColor);
			}

		}
	}

	void Renderer3D::RenderSkybox(const Ref<Cubemap>& vSkybox)
	{
		if (vSkybox) {
			RenderCommand::DepthMask(false);
			s_Data.m_SkyboxShader->bind();
			vSkybox->bind(0);
			s_Data.m_SkyboxShader->setInt("u_Skybox", 0);
			RenderCommand::DrawIndexed(s_Data.m_SkyboxVertexArray.get(), 36);
			s_Data.m_SkyboxShader->unbind();
			RenderCommand::DepthMask(true);
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
		s_Data.m_LineVertexDataBatch->reset();
		s_Data.m_LineIndexCount = 0;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

}