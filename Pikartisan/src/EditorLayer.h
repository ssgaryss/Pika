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
		void onUpdateEditor(Timestep vTimestep);
		void onUpdateRuntime(Timestep vTimestep);
		void onUpdateSimulation(Timestep vTimestep);

		void onImGuiRender() override;
		void onEvent(Event& vEvent) override;
	private:
		// Scene Manipulations
		void newScene();
		void openScene();
		void openScene(const std::filesystem::path& vScenePath);
		void saveScene();
		void saveSceneAs();
		//// Scene Operations
		//void onScenePlay();
		//void onSceneStop();
		//void onScenePause();
		//void onSceneSimulate();
	private:
		// Shortcuts
		void initializeShortcutLibrary();          // 初始化快捷键  TODO : 有Project序列化后读取Project中包含快捷键设置
		// Event callbacks
		bool onKeyPressed(KeyPressedEvent& vEvent);
		bool onMousePressed(MouseButtonPressedEvent& vEvent);
	private:
		enum class SceneState {
			Edit = 0,
			Play = 1,
			Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;
	private:
		// Viewport
		bool m_IsViewportFocus = false;
		bool m_IsViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f }; // Viewport可用区域长宽
		glm::vec2 m_ViewportBounds[2]{};           // Viewport可用区域的左上与右下点（屏幕绝对坐标）
		// Gizmo
		uint32_t m_GizmoType = 0;                  // Translate = ImGuizmo::OPERATION::TRANSLATE = 7, Rotation = 120, Scale = 896
		Entity m_MouseHoveredEntity;               // 鼠标悬停的Entity
		// Shortcuts
		ShortcutLibrary m_ShortcutLibrary;
	private:
		// Renderer
		EditorCamera m_EditorCamera = {};
		Ref<ShaderLibrary> m_ShaderLibrary;      // TODO : SceneRenderer
		Ref<Framebuffer> m_Framebuffer;          // TODO : SceneRenderer
		// Scenes and SceneHierarchyPanel
		std::filesystem::path m_ActiveScenePath; // TODO : Delete! use project path
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
