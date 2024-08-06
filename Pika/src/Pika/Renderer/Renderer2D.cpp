#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Pika {

#define MAX_QUADS_PER_BATCH 10000 // for now

	struct QuadVertexData {
		glm::vec3 m_Position;
		glm::vec4 m_Color;
		glm::vec2 m_TexCoord;
		float m_TextureIndex = 0.0f;
		float m_TilingFactor = 1.0f;
		// TODO : Editor only
		int m_EntityID;
	};

	struct Renderer2DData {
		static const uint32_t s_MaxQuadsPerBatch = MAX_QUADS_PER_BATCH;
		static const uint32_t s_MaxVerticesPerBatch = s_MaxQuadsPerBatch * 4;
		static const uint32_t s_MaxIndicesPerBatch = s_MaxQuadsPerBatch * 6;

		uint32_t m_QuadIndexCount = 0;
		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_QuadShader;
		glm::vec4 m_QuadUnitVertex[4];

		QuadVertexData* m_pQuadVertexBufferBase = nullptr;
		QuadVertexData* m_pQuadVertexBufferPtr = nullptr;

		//Textures
		friend void Renderer2D::Initialize();
		uint32_t getMaxTextureSlots() const { return m_MaxTextureSlots; }
		std::optional<uint32_t> findTextureIndex(const Ref<Texture2D>& vTexture) {  // 已存在Texture则返回其index
			for (uint32_t i = 1; i < m_TextureIndex; ++i) {
				if (*vTexture.get() == *m_TextureSlots[i].get()) {
					return i;
				}
			}
			return std::nullopt;
		}
		std::optional<uint32_t> addTexture(const Ref<Texture2D>& vTexture) {
			if (m_TextureIndex >= m_MaxTextureSlots) return std::nullopt;
			uint32_t TextureIndex = m_TextureIndex;
			m_TextureSlots[TextureIndex] = vTexture;
			m_TextureIndex++;
			return TextureIndex;
		}
		std::array<Ref<Texture2D>, 128> m_TextureSlots; // for now : 默认不多于128个texture
		Ref<Texture2D> m_WhiteTexture; // Default at texture slot 0
		uint32_t m_TextureIndex = 1;


		struct Camera2DData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		};
		Camera2DData m_Camera2DData;

		Renderer2D::Statistics m_Statistics; // Record the renderer states
	private:
		uint32_t m_MaxTextureSlots = 32; // Only can be reset by Renderer2D::Init()
	};

	static Renderer2DData s_Data;

	void Renderer2D::Initialize()
	{
		PK_PROFILE_FUNCTION();

		RenderCommand::Initialize();
		s_Data.m_QuadUnitVertex[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.m_QuadUnitVertex[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.m_QuadVertexArray = VertexArray::Create();
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
			{Pika::ShaderDataType::Int, "a_EntityID"}
		};
		s_Data.m_QuadVertexBuffer->setLayout(QuadLayout);
		s_Data.m_QuadVertexArray->addVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadShader = Shader::Create("assets/shaders/Renderer2D/DefaultQuadShader.glsl");

		s_Data.m_pQuadVertexBufferBase = new QuadVertexData[s_Data.s_MaxVerticesPerBatch];

		s_Data.m_MaxTextureSlots = RenderCommand::getAvailableTextureSlots();
		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;
	}

	void Renderer2D::BeginScene(const EditorCamera& vEditorCamera)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vEditorCamera.getViewProjectionMatrix();
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[32]; //基于Shader中变量的数据
		for (int32_t i = 0; i < 32; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);

		ResetStatistics();
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& vCamera, const glm::mat4& vTramsform)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCamera.getProjectionMatrix() * glm::inverse(vTramsform);
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[32]; //基于Shader中变量的数据
		for (int32_t i = 0; i < 32; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);

		ResetStatistics();
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera2DController& vCameraController)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCameraController.getCamera().getViewProjectionMatrix();
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[32]; //基于Shader中变量的数据
		for (int32_t i = 0; i < 32; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);
		s_Data.m_QuadShader->setMat4("u_ViewProjectionMatrix", s_Data.m_Camera2DData.m_ViewProjectionMatrix);

		ResetStatistics();
		StartBatch();
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
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray.get(), s_Data.m_QuadIndexCount);
		s_Data.m_Statistics.m_DrawCalls++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vColor)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const glm::vec4& vColor)
	{
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const glm::vec4& vColor)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::toMat4(glm::quat(glm::radians(vRotation))) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::toMat4(glm::quat(glm::radians(vRotation))) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vTexture, vTilingFactor, vTintColor);
	}

	// SubTexture2D
	void Renderer2D::DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor, const glm::vec4& vTintColor) {
		DrawQuad(glm::vec3(vPosition, 0.0f), vScale, vSubTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor, const glm::vec4& vTintColor) {
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vSubTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor, const glm::vec4& vTintColor) {
		DrawRotatedQuad(glm::vec3(vPosition, 0.0f), vScale, vRotation, vSubTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec3& vRotation, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor, const glm::vec4& vTintColor) {
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), vPosition) *
			glm::toMat4(glm::quat(glm::radians(vRotation))) *
			glm::scale(glm::mat4(1.0f), glm::vec3(vScale, 1.0f));
		DrawQuad(Transform, vSubTexture, vTilingFactor, vTintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& vTransform, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_QuadIndexCount >= s_Data.s_MaxIndicesPerBatch)
			NextBatch();

		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = vTransform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vColor;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = 0;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = 1.0f;
			s_Data.m_pQuadVertexBufferPtr->m_EntityID = -1;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;
		s_Data.m_Statistics.m_QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& vTransform, const Ref<Texture2D>& vTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_QuadIndexCount >= s_Data.s_MaxIndicesPerBatch)
			NextBatch();

		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		float TextureIndex = static_cast<float>(s_Data.findTextureIndex(vTexture).value_or(0.0f));
		if (TextureIndex == 0.0f) {
			if (s_Data.m_TextureIndex >= s_Data.getMaxTextureSlots()) {
				NextBatch();
				TextureIndex = static_cast<float>(s_Data.addTexture(vTexture).value());
			}
			auto Success = s_Data.addTexture(vTexture);
			if (Success.has_value())
				TextureIndex = static_cast<float>(Success.value());
			else
				PK_CORE_ERROR(R"(Renderer2D : Fail to add texture(ID = {0}) to slots)", vTexture->getRendererID());
		}

		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = vTransform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vTintColor;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = TextureIndex;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = vTilingFactor;
			s_Data.m_pQuadVertexBufferPtr->m_EntityID = -1;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6; // 6 indices per quad
		s_Data.m_Statistics.m_QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& vTransform, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor, const glm::vec4& vTintColor)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_QuadIndexCount >= s_Data.s_MaxIndicesPerBatch)
			NextBatch();

		auto Texture = vSubTexture->getTexture();
		auto TexCoord = vSubTexture->getTextureCoordinates();
		float TextureIndex = static_cast<float>(s_Data.findTextureIndex(Texture).value_or(0.0f));
		if (TextureIndex == 0.0f) {
			if (s_Data.m_TextureIndex >= s_Data.getMaxTextureSlots()) {
				NextBatch();
				TextureIndex = static_cast<float>(s_Data.addTexture(Texture).value());
			}
			auto Success = s_Data.addTexture(Texture);
			if (Success.has_value())
				TextureIndex = static_cast<float>(Success.value());
			else
				PK_CORE_ERROR(R"(Renderer2D : Fail to add texture(ID = {0}) to slots)", Texture->getRendererID());
		}

		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = vTransform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vTintColor;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = TextureIndex;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = vTilingFactor;
			s_Data.m_pQuadVertexBufferPtr->m_EntityID = -1;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;
		s_Data.m_Statistics.m_QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& vTransform, const SpriteRendererComponent& vSprite, int vEntityID)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_QuadIndexCount >= s_Data.s_MaxIndicesPerBatch)
			NextBatch();

		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = vTransform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vSprite.m_Color;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = 0;
			s_Data.m_pQuadVertexBufferPtr->m_TilingFactor = 1.0f;
			s_Data.m_pQuadVertexBufferPtr->m_EntityID = vEntityID;
			s_Data.m_pQuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;
		s_Data.m_Statistics.m_QuadCount++;
	}

	void Renderer2D::ResetStatistics()
	{
		std::memset(&s_Data.m_Statistics, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_Data.m_Statistics;
	}

	void Renderer2D::StartBatch()
	{

		s_Data.m_QuadIndexCount = 0;
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		s_Data.m_TextureIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

}