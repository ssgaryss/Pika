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
		static const uint32_t s_MaxTriangleVerticesPerBatch = s_MaxTrianglesPerBatch * 3; // �˴���Ϊ���ȴ���Buffer�������
		// StaticMesh
		Ref<VertexArray> m_StaticMeshVertexArray = nullptr;
		Ref<VertexBuffer> m_StaticMeshVertexBuffer = nullptr;
		Ref<Shader> m_StaticMeshShader = nullptr;
		Ref<Shader> m_BlinnPhoneShader = nullptr;
		Ref<UniformBuffer> m_BlinnPhoneMaterialDataUniformBuffer = nullptr;

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

		// Camera
		struct CameraUniformBufferData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f); // Skybox��Ⱦ��Ҫ
		};
		CameraUniformBufferData m_CameraData;
		Ref<UniformBuffer> m_CameraDataUniformBuffer = nullptr;

		// Lights
		static const uint32_t s_MaxDirectionLightsNumber = 1;
		static const uint32_t s_MaxPointLightsNumber = 4;
		static const uint32_t s_MaxSpotLightsNumber = 4;

		struct LightsUniformBufferData {
			struct UniformBufferSTD140PointLightData
			{
				// �����ڴ������Ϊ������std140��vec3���뵽16�ֽ�
				alignas(16) glm::vec3 m_Position = glm::vec3(0.0f);     // ��Դλ��
				alignas(16) glm::vec3 m_LightColor = glm::vec3(1.0f);   // ��Դ��ɫ
				float m_Intensity = 0.0f;                               // ��Դǿ��
				float m_Constant = 1.0f;                                // ����˥����
				float m_Linear = 0.07f;                                 // ����˥����
				float m_Quadratic = 0.017f;                             // ����˥����
				UniformBufferSTD140PointLightData() = default;
				void setData(const glm::vec3& vPosition, const PointLight::Data& vPointLightData) {
					m_Position = vPosition;
					m_LightColor = vPointLightData.m_LightColor;
					m_Intensity = vPointLightData.m_Intensity;
					m_Constant = vPointLightData.m_Constant;
					m_Linear = vPointLightData.m_Linear;
					m_Quadratic = vPointLightData.m_Quadratic;
				}
			};
			//std::array<UniformBufferSTD140PointLightData, 4> m_PointLightsData;
			UniformBufferSTD140PointLightData m_PointLightsData; // TODO : Delete��
		};
		LightsUniformBufferData m_LightsData;
		Ref<UniformBuffer> m_PointLightsDataUniformBuffer = nullptr;

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
			 1.0f,  1.0f,  1.0f, //7 // 8������
		};
		s_Data.m_SkyboxVertexBuffer = VertexBuffer::Create(SkyboxVertices, sizeof(SkyboxVertices));
		BufferLayout SkyboxLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
		};
		s_Data.m_SkyboxVertexBuffer->setLayout(SkyboxLayout);
		s_Data.m_SkyboxVertexArray->addVertexBuffer(s_Data.m_SkyboxVertexBuffer);
		s_Data.m_SkyboxShader = Shader::Create("resources/shaders/Renderer3D/DefaultSkyboxShader.glsl");
		s_Data.m_SkyboxVertexArray->unbind();

		s_Data.m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_CameraData), 0);     // glsl��binding = 0
		//TODO : Directional light
		s_Data.m_PointLightsDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_LightsData.m_PointLightsData), 2);
		//TODO : Spot light
		s_Data.m_BlinnPhoneMaterialDataUniformBuffer = UniformBuffer::Create(sizeof(BlinnPhoneMaterial::Data), 4); // ע������GLSL std140�ڴ����vec3��4bytes������������Ҫ�ֶ�����

		PK_CORE_INFO("Success to initialize Pika 3D Renderer!");
	}

	void Renderer3D::BeginScene(const EditorCamera& vEditorCamera, const LightsData& vLightsData)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_CameraData.m_ViewProjectionMatrix = vEditorCamera.getViewProjectionMatrix();
		s_Data.m_CameraData.m_ViewMatrix = vEditorCamera.getViewMatrix();
		s_Data.m_CameraData.m_ProjectionMatrix = vEditorCamera.getProjectionMatrix();
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));

		// Point Lights
		size_t PointLightsDataSize = vLightsData.m_PointLights.size();
		for (size_t i = 0; i < PointLightsDataSize; ++i) {
			// TODO : Not only 1!
			auto [Transform, Light] = vLightsData.m_PointLights[i];
			s_Data.m_LightsData.m_PointLightsData.m_Position = Transform.m_Position;
			if (auto pPointLight = dynamic_cast<PointLight*>(Light.m_Light.get())) {
				const auto& Data = pPointLight->getData();
				s_Data.m_LightsData.m_PointLightsData.m_LightColor = Data.m_LightColor;
				s_Data.m_LightsData.m_PointLightsData.m_Intensity = Data.m_Intensity;
				s_Data.m_LightsData.m_PointLightsData.m_Constant = Data.m_Constant;
				s_Data.m_LightsData.m_PointLightsData.m_Linear = Data.m_Linear;
				s_Data.m_LightsData.m_PointLightsData.m_Quadratic = Data.m_Quadratic;
			}
		}
		s_Data.m_PointLightsDataUniformBuffer->setData(&s_Data.m_LightsData.m_PointLightsData, 
			sizeof(s_Data.m_LightsData.m_PointLightsData));

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

	void Renderer3D::DrawStaticMesh(const TransformComponent& vTransform, const StaticMesh& vMesh, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		Ref<IndexBuffer> StaticMeshIndexBuffer = IndexBuffer::Create(vMesh.getIndicesData(), vMesh.getIndicesCount());
		s_Data.m_StaticMeshVertexArray->setIndexBuffer(StaticMeshIndexBuffer);
		auto TransformVertices = vMesh.getVertices();
		for (auto& Vertex : TransformVertices) {
			Vertex.m_Position = static_cast<glm::mat4>(vTransform) * glm::vec4(Vertex.m_Position, 1.0f);
			Vertex.m_Normal = glm::toMat4(glm::quat(glm::radians(vTransform.m_Rotation))) * glm::vec4(Vertex.m_Normal, 1.0f);
			Vertex.m_EntityID = vEntityID;
		}
		s_Data.m_StaticMeshVertexBuffer->setData(TransformVertices.data(), vMesh.getVerticesSize());

		s_Data.m_StaticMeshShader->bind();
		RenderCommand::DrawIndexed(s_Data.m_StaticMeshVertexArray.get(), StaticMeshIndexBuffer->getCount());
		s_Data.m_Statistics.m_MeshCount++;
	}

	void Renderer3D::DrawStaticMesh(const TransformComponent& vTransform, const StaticMesh& vMesh, const MaterialComponent& vMaterial, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		Ref<IndexBuffer> StaticMeshIndexBuffer = IndexBuffer::Create(vMesh.getIndicesData(), vMesh.getIndicesCount());
		s_Data.m_StaticMeshVertexArray->setIndexBuffer(StaticMeshIndexBuffer);
		auto TransformVertices = vMesh.getVertices();
		for (auto& Vertex : TransformVertices) {
			Vertex.m_Position = static_cast<glm::mat4>(vTransform) * glm::vec4(Vertex.m_Position, 1.0f);
			Vertex.m_Normal = glm::toMat4(glm::quat(glm::radians(vTransform.m_Rotation))) * glm::vec4(Vertex.m_Normal, 1.0f);
			Vertex.m_EntityID = vEntityID;
		}
		s_Data.m_StaticMeshVertexBuffer->setData(TransformVertices.data(), vMesh.getVerticesSize());

		if (auto BlinnPhone = dynamic_cast<BlinnPhoneMaterial*>(vMaterial.m_Material.get())) {
			s_Data.m_BlinnPhoneShader->bind();
			const auto& MaterialData = BlinnPhone->getData();
			s_Data.m_BlinnPhoneMaterialDataUniformBuffer->setData(&MaterialData, sizeof(MaterialData));
		}
		RenderCommand::DrawIndexed(s_Data.m_StaticMeshVertexArray.get(), StaticMeshIndexBuffer->getCount());
		s_Data.m_Statistics.m_MeshCount++;
	}

	void Renderer3D::DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		for (auto& Mesh : vModel.m_Model->getMeshes()) {
			DrawStaticMesh(vTransform, Mesh, vEntityID);
		}
	}

	void Renderer3D::DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, const MaterialComponent& vMaterial, int vEntityID)
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