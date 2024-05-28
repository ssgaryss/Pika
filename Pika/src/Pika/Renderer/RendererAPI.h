#pragma once

namespace Pika {

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

		static std::shared_ptr<RendererAPI> create();
		virtual void clear() = 0;

	private:
		static GraphicsAPI s_GraphicsAPI;
	};

}

