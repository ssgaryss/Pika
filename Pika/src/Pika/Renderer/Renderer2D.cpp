#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Pika {

#define MAX_QUADS_PER_BATCH 10000 // for now

	struct QuadVertexData {
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);
		glm::vec2 m_TexCoord = glm::vec2(0.0f);
		int m_TextureIndex = 0;
		float m_TilingFactor = 1.0f;
		// TODO : Editor only
		int m_EntityID = -1;
	};

	struct LineVertexData
	{
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);

		int m_EntityID = -1;
	};

	struct Renderer2DData {
	public:
		static const uint32_t s_MaxQuadsPerBatch = MAX_QUADS_PER_BATCH;
		static const uint32_t s_MaxVerticesPerBatch = s_MaxQuadsPerBatch * 4;
		static const uint32_t s_MaxIndicesPerBatch = s_MaxQuadsPerBatch * 6;

		// Quad
		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_QuadShader;
		glm::vec4 m_QuadUnitVertex[4] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		uint32_t m_QuadIndexCount = 0;
		QuadVertexData* m_pQuadVertexBufferBase = nullptr;
		QuadVertexData* m_pQuadVertexBufferPtr = nullptr;

		// Line
		Ref<VertexArray> m_LineVertexArray;
		Ref<VertexBuffer> m_LineVertexBuffer;
		Ref<Shader> m_LineShader;
		float m_LineThickness = 0.5f;

		uint32_t m_LineIndexCount = 0;
		LineVertexData* m_pLineVertexBufferBase = nullptr;
		LineVertexData* m_pLineVertexBufferPtr = nullptr;

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


		struct CameraData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		};
		CameraData m_CameraData;
		Ref<UniformBuffer> m_CameraDataUniformBuffer;

		Renderer2D::Statistics m_Statistics; // Record the renderer states
	public:
		~Renderer2DData() {
			delete[] m_pQuadVertexBufferBase;
			delete[] m_pLineVertexBufferBase;
		}
	private:
		uint32_t m_MaxTextureSlots = 32; // Only can be reset by Renderer2D::Init()
	};

	static Renderer2DData s_Data;

	void Renderer2D::Initialize()
	{
		PK_PROFILE_FUNCTION();

		RenderCommand::Initialize();

		// Quad
		s_Data.m_QuadVertexArray = VertexArray::Create();
		s_Data.m_QuadVertexArray->bind();
		uint32_t* QuadIndicesPerBatch = new uint32_t[Renderer2DData::s_MaxIndicesPerBatch];
		{
			uint32_t Offset = 0;
			for (uint32_t i = 0; i < Renderer2DData::s_MaxIndicesPerBatch; i += 6) {
				QuadIndicesPerBatch[i + 0] = Offset + 0;
				QuadIndicesPerBatch[i + 1] = Offset + 1;
				QuadIndicesPerBatch[i + 2] = Offset + 2;
				QuadIndicesPerBatch[i + 3] = Offset + 2;
				QuadIndicesPerBatch[i + 4] = Offset + 3;
				QuadIndicesPerBatch[i + 5] = Offset + 0;
				Offset += 4;
			}
		}
		Ref<IndexBuffer> pQuadIndexBuffer = IndexBuffer::Create(QuadIndicesPerBatch, Renderer2DData::s_MaxIndicesPerBatch);
		s_Data.m_QuadVertexArray->setIndexBuffer(pQuadIndexBuffer);
		delete[] QuadIndicesPerBatch;

		s_Data.m_QuadVertexBuffer = VertexBuffer::Create(Renderer2DData::s_MaxVerticesPerBatch * sizeof(QuadVertexData));
		BufferLayout QuadLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"},
			{Pika::ShaderDataType::Int,    "a_TextureIndex"},
			{Pika::ShaderDataType::Float,  "a_TilingFactor"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_QuadVertexBuffer->setLayout(QuadLayout);
		s_Data.m_QuadVertexArray->addVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadShader = Shader::Create("assets/shaders/Renderer2D/DefaultQuadShader.glsl");

		s_Data.m_pQuadVertexBufferBase = new QuadVertexData[Renderer2DData::s_MaxVerticesPerBatch];
		s_Data.m_QuadVertexArray->unbind();

		// Line
		s_Data.m_LineVertexArray = VertexArray::Create();
		s_Data.m_LineVertexArray->bind();
		uint32_t* LineIndicesPerBatch = new uint32_t[Renderer2DData::s_MaxIndicesPerBatch];
		{
			for (uint32_t i = 0; i < Renderer2DData::s_MaxIndicesPerBatch; i++) {
				LineIndicesPerBatch[i] = i;
			}
		}
		Ref<IndexBuffer> pLineIndexBuffer = IndexBuffer::Create(LineIndicesPerBatch, Renderer2DData::s_MaxIndicesPerBatch);
		s_Data.m_LineVertexArray->setIndexBuffer(pLineIndexBuffer);
		delete[] LineIndicesPerBatch;

		s_Data.m_LineVertexBuffer = VertexBuffer::Create(Renderer2DData::s_MaxVerticesPerBatch * sizeof(LineVertexData));
		BufferLayout LineLayout = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float4, "a_Color"},
			{Pika::ShaderDataType::Int,    "a_EntityID"}
		};
		s_Data.m_LineVertexBuffer->setLayout(LineLayout);
		s_Data.m_LineVertexArray->addVertexBuffer(s_Data.m_LineVertexBuffer);
		s_Data.m_LineShader = Shader::Create("assets/shaders/Renderer2D/DefaultLineShader.glsl");

		s_Data.m_pLineVertexBufferBase = new LineVertexData[Renderer2DData::s_MaxVerticesPerBatch];
		s_Data.m_LineVertexArray->unbind();

		// Default texture
		s_Data.m_MaxTextureSlots = RenderCommand::getAvailableTextureSlots();
		TextureSpecification TS;
		s_Data.m_WhiteTexture = Texture2D::Create(TS);
		uint32_t Data = 0xffffffff;
		s_Data.m_WhiteTexture->setData(&Data, sizeof(Data));
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;

		s_Data.m_CameraDataUniformBuffer = UniformBuffer::Create(sizeof(s_Data.m_CameraData), 0); // glsl中binding = 0
	}

	void Renderer2D::BeginScene(const EditorCamera& vEditorCamera)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_CameraData.m_ViewProjectionMatrix = vEditorCamera.getViewProjectionMatrix();
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[32]; //基于Shader中变量的数据
		for (int32_t i = 0; i < 32; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);

		ResetStatistics();
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& vCamera, const glm::mat4& vViewMatrix)
	{
		PK_PROFILE_FUNCTION();
		s_Data.m_CameraData.m_ViewProjectionMatrix = vCamera.getProjectionMatrix() * vViewMatrix;
		s_Data.m_CameraDataUniformBuffer->setData(&s_Data.m_CameraData, sizeof(s_Data.m_CameraData));
		s_Data.m_QuadShader->bind();
		// TODO : delete!!!
		int32_t Textures[32]; //基于Shader中变量的数据
		for (int32_t i = 0; i < 32; ++i)
			Textures[i] = i;
		s_Data.m_QuadShader->setIntArray("u_Textures", Textures, s_Data.m_TextureIndex);

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
		// Quads
		if (s_Data.m_QuadIndexCount) {
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
		if (s_Data.m_QuadIndexCount >= Renderer2DData::s_MaxIndicesPerBatch)
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
		if (s_Data.m_QuadIndexCount >= Renderer2DData::s_MaxIndicesPerBatch)
			NextBatch();

		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		int TextureIndex = static_cast<int>(s_Data.findTextureIndex(vTexture).value_or(0));
		if (TextureIndex == 0) {
			if (s_Data.m_TextureIndex >= s_Data.getMaxTextureSlots()) {
				NextBatch();
			}
			auto Success = s_Data.addTexture(vTexture);
			if (Success.has_value())
				TextureIndex = static_cast<int>(Success.value());
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
		if (s_Data.m_QuadIndexCount >= Renderer2DData::s_MaxIndicesPerBatch)
			NextBatch();

		auto Texture = vSubTexture->getTexture();
		auto TexCoord = vSubTexture->getTextureCoordinates();
		int TextureIndex = static_cast<int>(s_Data.findTextureIndex(Texture).value_or(0));
		if (TextureIndex == 0) {
			if (s_Data.m_TextureIndex >= s_Data.getMaxTextureSlots()) {
				NextBatch();
			}
			auto Success = s_Data.addTexture(Texture);
			if (Success.has_value())
				TextureIndex = static_cast<int>(Success.value());
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

	void Renderer2D::SetLineThickness(float vThickness)
	{
		s_Data.m_LineThickness = vThickness;
	}

	float Renderer2D::GetLineThickness()
	{
		return s_Data.m_LineThickness;
	}

	void Renderer2D::DrawLine(const glm::vec3& vStartPosition, const glm::vec3& vEndPosition, const glm::vec4& vColor)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_LineIndexCount >= Renderer2DData::s_MaxIndicesPerBatch)
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

	void Renderer2D::DrawGrid(const glm::mat4& vIdentityMatrix, float vSize, const glm::vec4& vColor, float vInterval)
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

			if (vIdentityMatrix != glm::mat4(1.0f)) {     // 一般只使用XOY平面Grid,节约算力
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

	void Renderer2D::DrawSprite(const glm::mat4& vTransform, const SpriteRendererComponent& vSprite, int vEntityID)
	{
		PK_PROFILE_FUNCTION();
		if (s_Data.m_QuadIndexCount >= Renderer2DData::s_MaxIndicesPerBatch)
			NextBatch();

		constexpr glm::vec2 TexCoord[4] = { {0.0f, 0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f} };

		int TextureIndex = 0;
		if (vSprite.m_Texture) {
			TextureIndex = static_cast<int>(s_Data.findTextureIndex(vSprite.m_Texture).value_or(0));
			if (TextureIndex == 0) {
				if (s_Data.m_TextureIndex >= s_Data.getMaxTextureSlots()) {
					NextBatch();
				}
				auto Success = s_Data.addTexture(vSprite.m_Texture);
				if (Success.has_value())
					TextureIndex = static_cast<int>(Success.value());
				else
					PK_CORE_ERROR(R"(Renderer2D : Fail to add texture(ID = {0}) to slots)", vSprite.m_Texture->getRendererID());
			}
		}
		for (int i = 0; i < 4; ++i) {
			s_Data.m_pQuadVertexBufferPtr->m_Position = vTransform * s_Data.m_QuadUnitVertex[i];
			s_Data.m_pQuadVertexBufferPtr->m_Color = vSprite.m_Color;
			s_Data.m_pQuadVertexBufferPtr->m_TexCoord = TexCoord[i];
			s_Data.m_pQuadVertexBufferPtr->m_TextureIndex = TextureIndex;
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
		// Quad
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_pQuadVertexBufferPtr = s_Data.m_pQuadVertexBufferBase;

		// Line
		s_Data.m_LineIndexCount = 0;
		s_Data.m_pLineVertexBufferPtr = s_Data.m_pLineVertexBufferBase;

		s_Data.m_TextureIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

}