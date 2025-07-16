#pragma once
#include "VertexArray.h"

namespace Pika {

	class RendererAPI
	{
	public:
		enum class GraphicsAPI {
			None = 0,
			OpenGL = 1,
			DirectX = 2,
			Vulkan = 3
		};
	public:
		enum RendererAPISettingFlags : uint32_t {
			None = 0,
			EnableBlend = 1 << 0,
			EnableDepthTest = 1 << 1,
			EnableLineSmooth = 1 << 2,
			EnableCullBackFace = 1 << 3
			// TODO : Others
		};
	public:
		inline static GraphicsAPI getAPI() { return s_GraphicsAPI; }
		inline static void setAPI(GraphicsAPI vGraphicsAPI) { s_GraphicsAPI = vGraphicsAPI; }

		virtual void initialize(uint32_t vFlags = 0) = 0;
		virtual void clear() = 0;
		virtual void setClearColor(const glm::vec4& vColor) = 0;
		virtual void drawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount) = 0;
		virtual void drawLines(const VertexArray* vVertexArray, uint32_t vIndexCount) = 0;
		virtual void setLineThickness(float vThickness) = 0;

		virtual uint32_t getAvailableTextureSlots() = 0;
		virtual void depthMask(bool vAllow) = 0;

		static Scope<RendererAPI> Create();
	private:
		static GraphicsAPI s_GraphicsAPI;
	};

}

