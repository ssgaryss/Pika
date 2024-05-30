#pragma once
#include "VertexArray.h"

namespace Pika {

	struct Color {
		float m_Red = 0.0f;
		float m_Green = 0.0f;
		float m_Blue = 0.0f;
		float m_Alpha = 1.0f;
		Color() = default;
		Color(float vRed, float vGreen, float vBlue, float vAlpha)
			:m_Red{ vRed }, m_Green{ vGreen }, m_Blue{ vBlue }, m_Alpha{ vAlpha } {}
	};

	class RendererAPI
	{
	public:
		enum class GraphicsAPI {
			None = 0,
			OpenGL = 1,
			DirectX = 2
		};

	public:
		inline static GraphicsAPI getAPI() { return s_GraphicsAPI; }
		inline static void setAPI(GraphicsAPI vGraphicsAPI) { s_GraphicsAPI = vGraphicsAPI; }

		virtual void clear() = 0;
		virtual void setClearColor() = 0;
		virtual void setClearColor(Color vColor) = 0;
		virtual void drawIndexed(const VertexArray* vVertexArray) = 0;

		static RendererAPI* Create();
	private:
		static GraphicsAPI s_GraphicsAPI;
	};

}

