#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Pika/Renderer/Framebuffer.h"
#include "Pika/Renderer/Shader.h"
#include "Pika/Renderer/EditorCamera.h"

namespace Pika {

	class SceneRenderer
	{
	public:
		struct RendererSettings
		{
			bool m_ShowGrid = true;
		};
	public:
		SceneRenderer() = default;
		SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer);

		void initialize();  // 初始化Renderer
		void beginFrame();
		void endFrame();

		void render();   // TODO : render(Camera)       // render with primary camera
		void render(const EditorCamera& vEditorCamera); // render with EditorCamera

		inline const Ref<Scene>& getContext() const { return m_Context; }
		inline void setContext(const Ref<Scene>& vScene) { m_Context = vScene; m_PrimaryCamera = {}; }
		inline const Ref<Framebuffer>& getFramebuffer() const { return m_Framebuffer; }
		inline void setFramebuffer(const Ref<Framebuffer>& vFramebuffer) { m_Framebuffer = vFramebuffer; }
		inline const Entity& getPrimaryCamera() const { return m_PrimaryCamera; }
		inline void setPrimaryCamera(const Entity& vCamera) { m_PrimaryCamera = vCamera; }
		inline Ref<Cubemap> getSkybox() const { return m_Context->m_Skybox; }
		inline void setSkybox(const Ref<Cubemap>& vSkybox) { m_Context->m_Skybox = vSkybox; }

		// Settings
		inline bool* showGrid() { return &m_Settings.m_ShowGrid; }


		void resize(uint32_t vWidth, uint32_t vHeight); // resize FBO
	private:
		RendererSettings m_Settings;
		Ref<Scene> m_Context;
		Ref<Framebuffer> m_Framebuffer;
		Ref<ShaderLibrary> m_ShaderLibray; // TODO : 自己创建的Shaders
		Entity m_PrimaryCamera = {};       // TOD0 : Renderer Should not own camera
		
	};

}
