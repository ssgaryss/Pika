#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Lights.h"
#include "RenderDataExtractor.h"
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

		void initialize();
		void beginFrame();
		void endFrame();

		void render();   // TODO : render(Camera)       // render with primary camera
		void render(const EditorCamera& vEditorCamera); // render with EditorCamera

		inline const Ref<RenderDataExtractor>& getRenderDataExtractor() const { return m_RenderDataExtractor; }
		inline void setRenderDataExtractor(const Ref<RenderDataExtractor>& vRenderDataExtractor) {
			m_RenderDataExtractor = vRenderDataExtractor;
			m_PrimaryCamera = {};
		}
		inline const Ref<Framebuffer>& getFramebuffer() const { return m_Framebuffer; }
		inline void setFramebuffer(const Ref<Framebuffer>& vFramebuffer) { m_Framebuffer = vFramebuffer; }
		inline const Entity& getPrimaryCamera() const { return m_PrimaryCamera; }
		inline void setPrimaryCamera(const Entity& vCamera) { m_PrimaryCamera = vCamera; }

		// Settings
		inline bool* showGrid() { return &m_Settings.m_ShowGrid; }

		void resize(uint32_t vWidth, uint32_t vHeight); // resize FBO
	private:
		RendererSettings m_Settings;
		Ref<RenderDataExtractor> m_RenderDataExtractor = nullptr;
		Ref<Framebuffer> m_Framebuffer = nullptr;
		Ref<ShaderLibrary> m_ShaderLibray = nullptr; // TODO : 自己创建的Shaders

		// TODO : Remove all Scene Data!
		Entity m_PrimaryCamera = {};       // TOD0 : Use C# to control it.
	};

}
