#pragma once
#include "Camera2DController.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Pika
{

	class Renderer2D
	{
	public:
		Renderer2D() = delete;
		static void Init();
		static void BeginScene(Camera2DController& vCameraController);
		static void EndScene();

		static void drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vTintColor);
		static void drawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const glm::vec4& vTintColor);
		static void drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& vPosition, const glm::vec2& vScale, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));

		static void drawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vTintColor);
		static void drawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const glm::vec4& vTintColor);
		static void drawRotatedQuad(const glm::vec2& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& vPosition, const glm::vec2& vScale, float vRotation, const Ref<Texture2D>& vTexture, float vTilingFactor = 1.0f, const glm::vec4& vTintColor = glm::vec4(1.0f));

	};

}
