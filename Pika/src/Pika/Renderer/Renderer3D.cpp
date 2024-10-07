#include "pkpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderBatch.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "Pika/Scene/Primitive.h"

namespace Pika {


	namespace Utils {

		static glm::mat4 getDirectionLightSpaceMatrix(const glm::vec3& vRotation, const DirectionLight::Data& vDirectionLightData) {
			const auto& Data = vDirectionLightData;
			glm::vec3 DefaultDirection = Data.s_DefaultDirection;
			glm::vec3 Direction = glm::toMat4(glm::quat(glm::radians(vRotation))) * glm::vec4(DefaultDirection, 1.0f);
			// Direction Light理论上是无限大，这里我不想和位置有关
			glm::mat4 LightViewMatrix = glm::lookAt(glm::vec3(0.0f), Direction,
				glm::rotate(glm::quat(glm::radians(glm::vec3(-vRotation.x, -vRotation.y, -vRotation.z))), glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::mat4 LightProjectionMatrix = glm::ortho(-Data.m_LightSize, Data.m_LightSize,
				-Data.m_LightSize, Data.m_LightSize, -Data.m_LightSize, Data.m_LightSize);
			return LightProjectionMatrix * LightViewMatrix;
		}

		static std::vector<glm::mat4> getPointLightSpaceMatrices(const glm::vec3& vPosition, float vLightSize) {
			std::vector<glm::mat4> LightSpaceMatrices;
			LightSpaceMatrices.reserve(6);
			float NearClip = 0.1f;
			float FarClip = vLightSize;
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, NearClip, FarClip);
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			LightSpaceMatrices.push_back(shadowProj * glm::lookAt(vPosition, vPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			return LightSpaceMatrices;
		}

	}


#define MAX_TRIANGLES_PER_BATCH 10000 // for now
#define MAX_LINES_PER_BATCH 10000 // for now

	struct Renderer3DData
	{
	public:
		static const uint32_t s_MaxTrianglesPerBatch = MAX_TRIANGLES_PER_BATCH;
		static const uint32_t s_MaxTriangleVerticesPerBatch = s_MaxTrianglesPerBatch * 3; // 此处是为了先创建Buffer提高性能
		static const uint32_t s_MaxTriangleIndicesPerBatch = s_MaxTrianglesPerBatch * 3;
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

		uint32_t m_LineIndexCount = 0; // Index Buffer 数据计数
		Ref<RenderBatch<LineVertexData>> m_LineVertexDataBatch = nullptr;

		// Skybox
		Ref<VertexArray> m_SkyboxVertexArray = nullptr;
		Ref<VertexBuffer> m_SkyboxVertexBuffer = nullptr;
		Ref<Shader> m_SkyboxShader = nullptr;

		// Textures
		static const uint32_t m_MaxTextureSlots = 28; // TODO : 这个应该和硬件有关！后四个暂时只给点光源Shadow用
		std::optional<uint32_t> findTextureIndex(const Ref<Texture2D>& vTexture) {  // 已存在Texture则返回其index
			for (uint32_t i = 1; i < m_TextureIndex; ++i) {
				if (m_TextureSlots[i]) {
					if (*vTexture.get() == *m_TextureSlots[i].get()) {
						return i;
					}
				}
			}
			return std::nullopt;
		}
		std::optional<uint32_t> addTexture(const Ref<Texture2D>& vTexture) {
			if (m_TextureIndex >= m_MaxTextureSlots)
				return std::nullopt;
			uint32_t TextureIndex = m_TextureIndex;
			m_TextureSlots[TextureIndex] = vTexture;
			m_TextureIndex++;
			return TextureIndex;
		}
		void resetTextureSlots() {
			for (uint32_t i = 1; i < m_MaxTextureSlots; ++i) {
				if (m_TextureSlots[i]) {
					m_TextureSlots[i]->unbind(i);
					m_TextureSlots[i] = nullptr;
				}
			}
			m_TextureIndex = 1;
			m_TextureSlots[0] = m_WhiteTexture;
		}
		void bindTextureSlots() {
			uint32_t SupportSlotsNum = std::min(m_TextureIndex, m_MaxTextureSlots);
			for (uint32_t i = 0; i < SupportSlotsNum; ++i) {
				if (m_TextureSlots[i])
					m_TextureSlots[i]->bind(i);
			}
		}
		std::array<Ref<Texture2D>, 128> m_TextureSlots; // for now : 默认不多于128个texture
		Ref<Texture2D> m_WhiteTexture; // Default at texture slot 0
		uint32_t m_TextureIndex = 1;

		// Camera
		struct CameraUniformBufferData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f); // Skybox渲染需要
		};
		CameraUniformBufferData m_CameraData;
		Ref<UniformBuffer> m_CameraDataUniformBuffer = nullptr;

		// Materials
		struct BlinnPhoneMaterialUniformBufferData
		{
			alignas(16) glm::vec3 m_Ambient = glm::vec3(1.0f);
			alignas(16) glm::vec3 m_Diffuse = glm::vec3(1.0f);
			alignas(16) glm::vec3 m_Specular = glm::vec3(1.0f);
			float m_Shininess = 5.0f;
			uint32_t m_DiffuseMapIndex = 0;
			uint32_t m_SpecularMapIndex = 0;
		};
		BlinnPhoneMaterialUniformBufferData m_BlinnPhoneMaterialUniformBufferData;

		void setBlinnPhoneMaterialData(const BlinnPhoneMaterial::Data& vBlinnPhoneMaterialData) {
			m_BlinnPhoneMaterialUniformBufferData.m_Ambient = vBlinnPhoneMaterialData.m_Ambient;
			m_BlinnPhoneMaterialUniformBufferData.m_Diffuse = vBlinnPhoneMaterialData.m_Diffuse;
			m_BlinnPhoneMaterialUniformBufferData.m_Specular = vBlinnPhoneMaterialData.m_Specular;
			m_BlinnPhoneMaterialUniformBufferData.m_Shininess = vBlinnPhoneMaterialData.m_Shininess;
			m_BlinnPhoneMaterialUniformBufferData.m_DiffuseMapIndex = 0;
			m_BlinnPhoneMaterialUniformBufferData.m_SpecularMapIndex = 0;
			if (vBlinnPhoneMaterialData.m_DiffuseMap) {
				uint32_t DiffuseTextureIndex = static_cast<uint32_t>(findTextureIndex(vBlinnPhoneMaterialData.m_DiffuseMap).value_or(0));
				if (DiffuseTextureIndex == 0) {
					auto Success = addTexture(vBlinnPhoneMaterialData.m_DiffuseMap);
					if (Success.has_value())
						DiffuseTextureIndex = static_cast<int>(Success.value());
					else
						PK_CORE_ERROR("Renderer3D : Fail to add Blinn-Phone diffuse texture to texture slots.");
				}
				m_BlinnPhoneMaterialUniformBufferData.m_DiffuseMapIndex = DiffuseTextureIndex;
			}
			if (vBlinnPhoneMaterialData.m_SpecularMap) {
				uint32_t SpecularTextureIndex = static_cast<uint32_t>(findTextureIndex(vBlinnPhoneMaterialData.m_SpecularMap).value_or(0));
				if (SpecularTextureIndex == 0) {
					auto Success = addTexture(vBlinnPhoneMaterialData.m_SpecularMap);
					if (Success.has_value())
						SpecularTextureIndex = static_cast<int>(Success.value());
					else
						PK_CORE_ERROR("Renderer3D : Fail to add Blinn-Phone specular texture to texture slots.");
				}
				m_BlinnPhoneMaterialUniformBufferData.m_SpecularMapIndex = SpecularTextureIndex;
			}
		}


		// Lights
		static const uint32_t s_MaxDirectionLightsNumber = 1;
		static const uint32_t s_MaxPointLightsNumber = 4;
		static const uint32_t s_MaxSpotLightsNumber = 4;

		struct LightsUniformBufferData {

			struct DirectionLightUniformBufferData
			{
				glm::mat4 m_LightSpaceMatrix = glm::mat4(1.0f); // std140中mat放前面，不然有问题，具体原因我还不知道
				alignas(16) glm::vec3 m_Direction = glm::vec3(0.0f);    // 方向
				alignas(16) glm::vec3 m_LightColor = glm::vec3(1.0f);   // 光源颜色
				float m_Intensity = 0.0f;                               // 光源强度
				int32_t m_ShadowMapIndex = -1;

				void setData(const glm::vec3& vDirection, const DirectionLight::Data& vDirectionLightData) {
					m_Direction = vDirection;
					m_LightColor = vDirectionLightData.m_LightColor;
					m_Intensity = vDirectionLightData.m_Intensity;
				}
			};
			struct PointLightUniformBufferData
			{
				// 这里内存对齐是为了满足std140中vec3对齐到16字节
				alignas(16) glm::vec3 m_Position = glm::vec3(0.0f);     // 光源位置
				alignas(16) glm::vec3 m_LightColor = glm::vec3(1.0f);   // 光源颜色
				float m_Intensity = 0.0f;                               // 光源强度
				float m_Constant = 1.0f;                                // 常数衰减项
				float m_Linear = 0.07f;                                 // 线性衰减项
				float m_Quadratic = 0.017f;                             // 二次衰减项
				int32_t m_ShadowMapIndex = -1;
				float m_LightSize = 5.0f;                               // Shadow的FarClip
				void setData(const glm::vec3& vPosition, const PointLight::Data& vPointLightData) {
					m_Position = vPosition;
					m_LightColor = vPointLightData.m_LightColor;
					m_Intensity = vPointLightData.m_Intensity;
					m_Constant = vPointLightData.m_Constant;
					m_Linear = vPointLightData.m_Linear;
					m_Quadratic = vPointLightData.m_Quadratic;
				}
			};

			std::array<DirectionLightUniformBufferData, s_MaxDirectionLightsNumber> m_DirectionLightsData;
			std::array<PointLightUniformBufferData, s_MaxPointLightsNumber> m_PointLightsData;
			// TODO : Spot Lights
		};
		LightsUniformBufferData m_LightsData;

		void setLightsData(const LightsData& vLightsData) {
			// Direction Lights
			uint32_t DirectionLightsDataSize = static_cast<uint32_t>(vLightsData.m_DirectionLights.size());
			if (DirectionLightsDataSize > s_MaxDirectionLightsNumber)
				PK_CORE_WARN("Renderer3D : There are {} direction lights, but renderer only support {}.",
					DirectionLightsDataSize, s_MaxDirectionLightsNumber);
			for (uint32_t i = 0; i < s_MaxDirectionLightsNumber; ++i) {
				if (i < DirectionLightsDataSize) {
					auto [Transform, Light] = vLightsData.m_DirectionLights[i];
					if (auto pDirectionLight = dynamic_cast<DirectionLight*>(Light.m_Light.get())) {
						const auto& Data = pDirectionLight->getData();
						glm::vec3 DefaultDirection = Data.s_DefaultDirection;
						m_LightsData.m_DirectionLightsData[i].m_Direction = glm::toMat4(glm::quat(glm::radians(Transform.m_Rotation))) * glm::vec4(DefaultDirection, 1.0f);
						m_LightsData.m_DirectionLightsData[i].m_LightColor = Data.m_LightColor;
						m_LightsData.m_DirectionLightsData[i].m_Intensity = Data.m_Intensity;
						m_LightsData.m_DirectionLightsData[i].m_ShadowMapIndex = Data.m_ShadowMap ? i : -1;
						if (Data.m_ShadowMap)
							m_LightsData.m_DirectionLightsData[i].m_LightSpaceMatrix = Utils::getDirectionLightSpaceMatrix(Transform.m_Rotation, Data);
						m_DirectionLightShadowMaps[i] = Data.m_ShadowMap;
					}
				}
				else {
					m_LightsData.m_DirectionLightsData[i].m_Intensity = 0.0f; // 重置
				}
			}
			// Point Lights
			uint32_t PointLightsDataSize = static_cast<uint32_t>(vLightsData.m_PointLights.size());
			if (PointLightsDataSize > s_MaxPointLightsNumber)
				PK_CORE_WARN("Renderer3D : There are {} point lights, but renderer only support {}.",
					PointLightsDataSize, s_MaxPointLightsNumber);
			for (uint32_t i = 0; i < s_MaxPointLightsNumber; ++i) {
				if (i < PointLightsDataSize) {
					auto [Transform, Light] = vLightsData.m_PointLights[i];
					m_LightsData.m_PointLightsData[i].m_Position = Transform.m_Position;
					if (auto pPointLight = dynamic_cast<PointLight*>(Light.m_Light.get())) {
						const auto& Data = pPointLight->getData();
						m_LightsData.m_PointLightsData[i].m_LightColor = Data.m_LightColor;
						m_LightsData.m_PointLightsData[i].m_Intensity = Data.m_Intensity;
						m_LightsData.m_PointLightsData[i].m_Constant = Data.m_Constant;
						m_LightsData.m_PointLightsData[i].m_Linear = Data.m_Linear;
						m_LightsData.m_PointLightsData[i].m_Quadratic = Data.m_Quadratic;
						m_LightsData.m_PointLightsData[i].m_ShadowMapIndex = Data.m_ShadowMap ? i : -1;
						m_LightsData.m_PointLightsData[i].m_LightSize = Data.m_LightSize;
						m_PointLightShadowMaps[i] = Data.m_ShadowMap;
					}
				}
				else {
					m_LightsData.m_PointLightsData[i].m_Intensity = 0.0f; // 重置
				}
			}
		}

		Ref<UniformBuffer> m_DirectionLightsDataUniformBuffer = nullptr;
		Ref<UniformBuffer> m_PointLightsDataUniformBuffer = nullptr;

		// Shadow
		static const uint32_t s_MaxDirectionLightShadowNumber = 1;
		static const uint32_t s_MaxPointLightShadowNumber = 4;
		Ref<VertexArray> m_VertexPositionArray = nullptr;
		Ref<VertexBuffer> m_VertexPositionBuffer = nullptr;
		Ref<Shader> m_Texture2DShadowMapShader = nullptr;
		Ref<Shader> m_CubemapShadowMapShader = nullptr;
		uint32_t m_VertexPositionIndexCount = 0; // Index Buffer 数据计数
		Ref<RenderBatch<StaticMeshVertexData>> m_VertexPositionDataBatch = nullptr;
		Ref<Framebuffer> m_ShadowMapBuffer = nullptr;
		std::array<Ref<Texture2D>, s_MaxDirectionLightShadowNumber> m_DirectionLightShadowMaps;
		std::array<Ref<Cubemap>, s_MaxPointLightShadowNumber> m_PointLightShadowMaps;
		void bindShadowMaps() {
			for (uint32_t i = 0; i < s_MaxDirectionLightShadowNumber; ++i) {
				if (m_DirectionLightShadowMaps[i]) {
					m_DirectionLightShadowMaps[i]->bind(27 + i);
				}
			}
			for (uint32_t i = 0; i < s_MaxPointLightShadowNumber; ++i) {
				if (m_PointLightShadowMaps[i]) {
					m_PointLightShadowMaps[i]->bind(27 + s_MaxDirectionLightShadowNumber + i);
				}
			}
		}

		Renderer3D::Statistics m_Statistics; // Record the renderer states
	};

	static Renderer3DData s_Data;

	void Renderer3D::Initialize()
	{
		PK_PROFILE_FUNCTION();
		PK_CORE_TRACE("Try to initialize Pika 3D Renderer ...");

		{
			uint32_t Flags = RendererAPI::EnableBlend | RendererAPI::EnableDepthTest
				| RendererAPI::EnableLineSmooth | RendererAPI::EnableCullBackFace;
			RenderCommand::Initialize(Flags);
		}

		// Reset Data
		s_Data.m_LightsData = {};
		s_Data.m_DirectionLightShadowMaps = {};
		s_Data.m_PointLightShadowMaps = {};
		s_Data.m_BlinnPhoneMaterialUniformBufferData = {};

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
		s_Data.m_BlinnPhoneShader->bind();
		std::vector<int32_t> Textures(s_Data.m_MaxTextureSlots);
		for (int32_t i = 0; i < s_Data.m_MaxTextureSlots; ++i)
			Textures[i] = i;
		s_Data.m_BlinnPhoneShader->setIntArray("u_Textures", Textures.data(), static_cast<uint32_t>(Textures.size()));
		s_Data.m_BlinnPhoneShader->setInt("u_DirectionLightShadowMap", 27);
		std::vector<int32_t> PointLightShadows(s_Data.s_MaxPointLightShadowNumber);
		for (int32_t i = 0; i < s_Data.s_MaxPointLightShadowNumber; ++i)
			PointLightShadows[i] = 28 + i;
		s_Data.m_BlinnPhoneShader->setIntArray("u_PointLightShadowMaps", PointLightShadows.data(), static_cast<uint32_t>(PointLightShadows.size()));
		s_Data.m_BlinnPhoneShader->unbind();
		s_Data.m_StaticMeshVertexArray->unbind();

		// Line
		s_Data.m_LineVertexDataBatch = CreateRef<RenderBatch<LineVertexData>>(Renderer3DData::s_MaxLineVerticesPerBatch);
		s_Data.m_LineVertexArray = VertexArray::Create();
		s_Data.m_LineVertexArray->bind();
		std::vector<uint32_t> Indices(Renderer3DData::s_MaxLineIndicesPerBatch);
		std::iota(Indices.begin(), Indices.end(), 0);
		Ref<IndexBuffer> LineIndexBuffer = IndexBuffer::Create(Indices.data(), Renderer3DData::s_MaxLineIndicesPerBatch);
		s_Data.m_LineVertexArray->setIndexBuffer(LineIndexBuffer);

		s_Data.m_LineVertexBuffer = VertexBuffer::Create(Renderer3DData::s_MaxLineVerticesPerBatch * sizeof(LineVertexData));
		BufferLayout LineLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_LineVertexBuffer->setLayout(LineLayout);
		s_Data.m_LineVertexArray->addVertexBuffer(s_Data.m_LineVertexBuffer);
		s_Data.m_LineShader = Shader::Create("resources/shaders/Renderer3D/DefaultLineShader.glsl");
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

		// Texture
		s_Data.resetTextureSlots();
		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
		s_Data.resetTextureSlots();

		// Uniform Buffers
		s_Data.m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_CameraData), 0);     // glsl中binding = 0
		s_Data.m_DirectionLightsDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_LightsData.m_DirectionLightsData), 1);
		s_Data.m_PointLightsDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_LightsData.m_PointLightsData), 2);
		//TODO : Spot light
		s_Data.m_BlinnPhoneMaterialDataUniformBuffer = UniformBuffer::Create(sizeof(BlinnPhoneMaterial::Data), 4); // 注意由于GLSL std140内存对齐vec3是4bytes，所以这里需要手动计算

		// Shadow
		s_Data.m_VertexPositionArray = VertexArray::Create();
		s_Data.m_VertexPositionArray->bind();
		s_Data.m_VertexPositionBuffer = VertexBuffer::Create(Renderer3DData::s_MaxTriangleVerticesPerBatch * sizeof(StaticMeshVertexData));
		BufferLayout VertexPositionLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float3, "a_Normal"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_VertexPositionBuffer->setLayout(VertexPositionLayout);
		s_Data.m_VertexPositionArray->addVertexBuffer(s_Data.m_VertexPositionBuffer);
		s_Data.m_Texture2DShadowMapShader = Shader::Create("resources/shaders/Renderer3D/Texture2DShadowMap.glsl");
		s_Data.m_CubemapShadowMapShader = Shader::Create("resources/shaders/Renderer3D/CubemapShadowMap.glsl");
		s_Data.m_VertexPositionDataBatch = CreateRef<RenderBatch<StaticMeshVertexData>>(Renderer3DData::s_MaxTriangleVerticesPerBatch);
		s_Data.m_ShadowMapBuffer = Framebuffer::Create({ 2048, 2048, 1,
			{ TextureFormat::DEPTH24STENCIL8 }, false });
		s_Data.m_VertexPositionArray->unbind();

		PK_CORE_INFO("Success to initialize Pika 3D Renderer!");
	}

	void Renderer3D::BeginScene(const EditorCamera& vEditorCamera, const LightsData& vLightsData)
	{
		PK_PROFILE_FUNCTION();

		s_Data.m_CameraData.m_ViewProjectionMatrix = vEditorCamera.getViewProjectionMatrix();
		s_Data.m_CameraData.m_ViewMatrix = vEditorCamera.getViewMatrix();
		s_Data.m_CameraData.m_ProjectionMatrix = vEditorCamera.getProjectionMatrix();
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));

		s_Data.setLightsData(vLightsData);
		s_Data.resetTextureSlots();
		s_Data.bindShadowMaps();
		s_Data.m_DirectionLightsDataUniformBuffer->setData(&s_Data.m_LightsData.m_DirectionLightsData,
			sizeof(s_Data.m_LightsData.m_DirectionLightsData));
		s_Data.m_PointLightsDataUniformBuffer->setData(&s_Data.m_LightsData.m_PointLightsData,
			sizeof(s_Data.m_LightsData.m_PointLightsData));

		ResetStatistics();
		StartBatch();
	}
	
	void Renderer3D::BeginScene(const Camera& vCamera, const glm::mat4& vViewMatrix, const LightsData& vLightsData)
	{
		PK_PROFILE_FUNCTION();
		
		s_Data.m_CameraData.m_ViewProjectionMatrix = vCamera.getProjectionMatrix() * vViewMatrix;
		s_Data.m_CameraData.m_ViewMatrix = vViewMatrix;
		s_Data.m_CameraData.m_ProjectionMatrix = vCamera.getProjectionMatrix();
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));

		s_Data.setLightsData(vLightsData);
		s_Data.resetTextureSlots();
		s_Data.bindShadowMaps();
		s_Data.m_DirectionLightsDataUniformBuffer->setData(&s_Data.m_LightsData.m_DirectionLightsData,
			sizeof(s_Data.m_LightsData.m_DirectionLightsData));
		s_Data.m_PointLightsDataUniformBuffer->setData(&s_Data.m_LightsData.m_PointLightsData,
			sizeof(s_Data.m_LightsData.m_PointLightsData));

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
			s_Data.m_LineShader->unbind();
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
		s_Data.m_Statistics.m_DrawCalls++;
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

		s_Data.bindTextureSlots();
		RenderCommand::DrawIndexed(s_Data.m_StaticMeshVertexArray.get(), StaticMeshIndexBuffer->getCount());
		s_Data.m_Statistics.m_DrawCalls++;
		s_Data.m_Statistics.m_MeshCount++;
	}

	void Renderer3D::DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		if (vModel.m_Model) {
			for (auto& Mesh : vModel.m_Model->getMeshes()) {
				DrawStaticMesh(vTransform, Mesh, vEntityID);
			}
		}
	}

	void Renderer3D::DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, const MaterialComponent& vMaterial, int vEntityID)
	{
		PK_PROFILE_FUNCTION();

		if (vModel.m_Model) {

			if (auto pBlinnPhoneMaterial = dynamic_cast<BlinnPhoneMaterial*>(vMaterial.m_Material.get())) {
				s_Data.m_BlinnPhoneShader->bind();
				const auto& MaterialData = pBlinnPhoneMaterial->getData();
				s_Data.setBlinnPhoneMaterialData(MaterialData);
				s_Data.m_BlinnPhoneMaterialDataUniformBuffer->setData(&s_Data.m_BlinnPhoneMaterialUniformBufferData,
					sizeof(s_Data.m_BlinnPhoneMaterialUniformBufferData));
			}

			for (auto& Mesh : vModel.m_Model->getMeshes()) {
				DrawStaticMesh(vTransform, Mesh, vMaterial, vEntityID);
			}
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

	void Renderer3D::DrawSkybox(const Ref<Cubemap>& vSkybox)
	{
		if (vSkybox) {
			RenderCommand::DepthMask(false);
			s_Data.m_SkyboxShader->bind();
			vSkybox->bind(0);
			s_Data.m_SkyboxShader->setInt("u_Skybox", 0);
			RenderCommand::DrawIndexed(s_Data.m_SkyboxVertexArray.get(), 36);
			s_Data.m_Statistics.m_DrawCalls++;
			s_Data.m_SkyboxShader->unbind();
			RenderCommand::DepthMask(true);
		}
	}

	void Renderer3D::DrawVerticesPosition(const glm::mat4& vTransform, const StaticMesh& vMesh)
	{
		PK_PROFILE_FUNCTION();
		try {
			uint32_t IndicesNumber = vMesh.getIndicesCount();
			auto TransformVertices = vMesh.getVertices();
			for (auto& Vertex : TransformVertices) {
				Vertex.m_Position = static_cast<glm::mat4>(vTransform) * glm::vec4(Vertex.m_Position, 1.0f);
			}
			s_Data.m_VertexPositionDataBatch->add(TransformVertices, vMesh.getIndices());
			s_Data.m_VertexPositionIndexCount += IndicesNumber;
		}
		catch (const std::runtime_error& e) {
			PK_CORE_WARN("RenderBatch : {}", e.what());
		}
	}

	void Renderer3D::DrawShadowMaps(const LightsData& vLightsData, const SceneData& vSceneData)
	{
		PK_PROFILE_FUNCTION();
		// TODO : 暂时认为Batch足够容纳整个场景数据
		if (!vLightsData.empty()) {
			// Start Batch
			s_Data.m_VertexPositionDataBatch->reset();
			s_Data.m_VertexPositionIndexCount = 0;

			// Scene Data
			for (const auto& ModelData : vSceneData.m_Models) {
				const auto& Model = std::get<1>(ModelData);
				if (Model.m_Model) {
					const auto& Transform = std::get<0>(ModelData);
					const std::vector<StaticMesh> StaticMeshes = Model.m_Model->getMeshes();
					for (const auto& StaticMesh : StaticMeshes) {
						DrawVerticesPosition(Transform, StaticMesh);
					}
				}
			}
			// Mesh及索引数据设置
			// TODO : 暂未考虑Batch不足flush的情况！
			s_Data.m_VertexPositionBuffer->setData(s_Data.m_VertexPositionDataBatch->data(), s_Data.m_VertexPositionDataBatch->size());
			const std::vector<uint32_t> Indices = s_Data.m_VertexPositionDataBatch->getIndices();
			uint32_t IndicesCount = s_Data.m_VertexPositionDataBatch->getIndicesCount();
			Ref<IndexBuffer> VertexPositionIndexBuffer = IndexBuffer::Create(Indices.data(), IndicesCount);
			s_Data.m_VertexPositionArray->setIndexBuffer(VertexPositionIndexBuffer);

			// Direction Light
			if (!vLightsData.m_DirectionLights.empty()) {
				uint32_t DirectionLightsNumber = static_cast<uint32_t>(vLightsData.m_DirectionLights.size());
				uint32_t ShadowMapsNumber = 0;
				for (uint32_t i = 0; i < s_Data.s_MaxDirectionLightsNumber; ++i) {
					if (i < DirectionLightsNumber && ShadowMapsNumber < s_Data.s_MaxDirectionLightShadowNumber) {
						const auto& Transform = std::get<0>(vLightsData.m_DirectionLights[i]);
						const auto& Light = std::get<1>(vLightsData.m_DirectionLights[i]).m_Light;
						if (auto pDirectionLight = dynamic_cast<DirectionLight*>(Light.get())) {
							auto& Data = pDirectionLight->getData();
							if (Data.m_EnableShadow) {
								// 绑定Shadow Map到ShadowMapBuffer
								s_Data.m_ShadowMapBuffer->bind();
								const auto& ShadowBufferInfo = s_Data.m_ShadowMapBuffer->getFramebufferSpecification();
								if (!Data.m_ShadowMap) {
									Data.m_ShadowMap = Texture2D::Create({ ShadowBufferInfo.m_Width, ShadowBufferInfo.m_Height,
										TextureFormat::DEPTH24STENCIL8, false });
								}
								s_Data.m_ShadowMapBuffer->setDepthStencilAttachment(Data.m_ShadowMap);
								// 渲染Shadow Map
								s_Data.m_Texture2DShadowMapShader->bind();
								glm::mat4 LightSpaceMatrix = Utils::getDirectionLightSpaceMatrix(Transform.m_Rotation, Data);
								s_Data.m_Texture2DShadowMapShader->setMat4("u_LightSpaceMatrix", LightSpaceMatrix);
								RenderCommand::Clear();
								RenderCommand::DrawIndexed(s_Data.m_VertexPositionArray.get(), s_Data.m_VertexPositionIndexCount);
								// 结束
								s_Data.m_Statistics.m_DrawCalls++;
								s_Data.m_Texture2DShadowMapShader->unbind();
								s_Data.m_ShadowMapBuffer->unbind();

								ShadowMapsNumber++;
							}
							else {
								Data.m_ShadowMap = nullptr;
								continue;
							}
						}
					}
				}
			}

			// Point Light
			if (!vLightsData.m_PointLights.empty()) {
				uint32_t PointLightsNumber = static_cast<uint32_t>(vLightsData.m_PointLights.size());
				uint32_t ShadowMapsNumber = 0;
				for (uint32_t i = 0; i < s_Data.s_MaxPointLightsNumber; ++i) {
					if (i < PointLightsNumber && ShadowMapsNumber < s_Data.s_MaxPointLightShadowNumber) {
						const auto& Transform = std::get<0>(vLightsData.m_PointLights[i]);
						const auto& Light = std::get<1>(vLightsData.m_PointLights[i]).m_Light;
						if (auto pPointLight = dynamic_cast<PointLight*>(Light.get())) {
							auto& Data = pPointLight->getData();
							if (Data.m_EnableShadow) {
								// 绑定Shadow Map到ShadowMapBuffer
								s_Data.m_ShadowMapBuffer->bind();
								const auto& ShadowBufferInfo = s_Data.m_ShadowMapBuffer->getFramebufferSpecification();
								if (!Data.m_ShadowMap) {
									Data.m_ShadowMap = Cubemap::Create({ ShadowBufferInfo.m_Width, ShadowBufferInfo.m_Height,
										TextureFormat::DEPTH24STENCIL8, false });
								}
								s_Data.m_ShadowMapBuffer->setDepthStencilAttachment(Data.m_ShadowMap);
								// 渲染Shadow Map
								s_Data.m_CubemapShadowMapShader->bind();
								auto LightSpaceMatrices = Utils::getPointLightSpaceMatrices(Transform.m_Position, Data.m_LightSize);
								s_Data.m_CubemapShadowMapShader->setFloat3("u_PointLightPosition", Transform.m_Position);
								s_Data.m_CubemapShadowMapShader->setFloat("u_LightSize", Data.m_LightSize);
								for (uint32_t i = 0; i < 6; ++i)
									s_Data.m_CubemapShadowMapShader->setMat4(std::format("u_LightSpaceMatrices[{0}]", i), LightSpaceMatrices[i]);
								RenderCommand::Clear();
								RenderCommand::DrawIndexed(s_Data.m_VertexPositionArray.get(), s_Data.m_VertexPositionIndexCount);
								// 结束
								s_Data.m_Statistics.m_DrawCalls++;
								s_Data.m_CubemapShadowMapShader->unbind();
								s_Data.m_ShadowMapBuffer->unbind();

								ShadowMapsNumber++;
							}
							else {
								Data.m_ShadowMap = nullptr;
								continue;
							}
						}
					}
				}
			}
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
		// Lines
		s_Data.m_LineVertexDataBatch->reset();
		s_Data.m_LineIndexCount = 0;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

}