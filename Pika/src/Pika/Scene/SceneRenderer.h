#pragma once
#include "Scene.h"
#include "Pika/Renderer/Framebuffer.h"
#include "Pika/Renderer/Shader.h"
#include "Pika/Renderer/EditorCamera.h"

namespace Pika {

	class SceneRenderer
	{
	public:
		SceneRenderer() = default;
		SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer);

		void beginFrame();
		void endFrame();
		void render(); // render with primary camera
		void render(const EditorCamera& vEditorCamera); // render with other camera

		inline const Ref<Framebuffer>& getFramebuffer() const { return m_Framebuffer; }
		inline const Ref<Scene>& getScene() const { return m_Scene; }
		inline void setScene(const Ref<Scene>& vScene) { m_Scene = vScene; }
		inline void setFramebuffer(const Ref<Framebuffer>& vFramebuffer) { m_Framebuffer = vFramebuffer; }

		void resize(uint32_t vWidth, uint32_t vHeight); // resize FBO
	private:
		void initialize();
	private:
		Ref<Scene> m_Scene;
		Ref<Framebuffer> m_Framebuffer;
		Ref<ShaderLibrary> m_ShaderLibray; // TODO : 自己创建的Shaders
	};

}
