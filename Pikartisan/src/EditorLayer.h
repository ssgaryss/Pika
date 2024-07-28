#pragma once
#include <Pika.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Pika
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;
		void onAttach() override;
		void onDetach() override;
		void onUpdate(Timestep vTimestep) override;
		void onImGuiRender() override;
		void onEvent(Event& vEvent) override;
	private:
		bool onKeyPressed(KeyPressedEvent& vEvent);
	private:
		// Viewport
		bool m_IsViewportFocus = false;
		bool m_IsViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f }; // Viewport可用区域长宽
		glm::vec2 m_ViewportBounds[2]; // Viewport可用区域的左上与右下点（屏幕绝对坐标）
		// Gizmo
		uint32_t m_GizmoType = 0;    // Translate = 1, Rotation = 2, Scale = 3
		Entity m_MouseHoveredEntity; // 鼠标悬停的Entity
	private:
		// Renderer
		Camera2DController m_CameraController;
		Ref<ShaderLibrary> m_ShaderLibrary;
		Ref<Framebuffer> m_Framebuffer;
		// Scenes and SceneHierarchyPanel
		Ref<Scene> m_ActiveScene;
		std::vector<Ref<Scene>> m_Scenes; // TODO : Not use it yet(only one FBO for now)
		// Panels
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel; // TODO : Mutiple Scenes
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		// TODO : remove these!
		Ref<Texture2D> m_TextureBackround;
		Ref<Texture2D> m_Texture2024;
		Ref<Texture2D> m_TextureRPGpack_sheet_2X;
		Ref<SubTexture2D> m_TextureTree;
		Ref<SubTexture2D> m_TextureWater, m_TextureGround;

		// TODO : Delete!
		float Rotation = 0.0f; //for now
	};

}
