#pragma once
#include "Camera.h"
#include "Camera2DController.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include <glm/glm.hpp>

namespace Pika
{

	class Renderer2D
	{
	public:
		Renderer2D() = delete;
		static void Init();
		// TODO : BeginScene should begin with all scene data!
		static void BeginScene(const Camera& vCamera, const glm::mat4& vTramsform);
		static void BeginScene(const Camera2DController& vCameraController);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor);
		static void DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vColor);
		static void DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor);
		static void DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vColor);
		static void DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		
		static void DrawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& vTransform, const glm::vec4& vColor);
		static void DrawQuad(const glm::mat4& vTransform, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& vTransform, const Ref<SubTexture2D>& vSubTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));

		struct Statistics {
			uint32_t m_DrawCalls = 0;
			uint32_t m_QuadCount = 0;
			uint32_t getDrawCalls() const { return m_DrawCalls; }
			uint32_t getQuadCount() const { return m_QuadCount; }
		};
		static void ResetStatistics();
		static Statistics GetStatistics();
	private:
		static void StartBatch();
		static void NextBatch();
	};

}
