#pragma once
#include "Scene.h"
#include "Pika/Renderer/Camera.h"
#include "Pika/Renderer/Framebuffer.h"
#include "Pika/Renderer/Shader.h"

namespace Pika {

	class SceneRenderer
	{
	public:
		SceneRenderer() = default;
		SceneRenderer(const Ref<Scene>& vScene);

		void render(const Camera& vCamera);
	private:
		enum RendererType {
			Renderer2D = 0,
			Renderer3D = 1
		};
		RendererType m_RendererType = RendererType::Renderer2D; // TODO : 3D
		void initialize();
	private:
		Ref<Scene> m_Scene;
		Scope<Framebuffer> m_Framebuffer;
		Ref<ShaderLibrary> m_ShaderLibray; // TODO : 自己创建的Shaders
	};

}
