#pragma once
#include <Pika.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneStatePanel.h"

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
		// Scene Manipulations
		void newScene(const std::string& vName = "Untitled", Scene::SceneType vType = Scene::SceneType::Scene2D);
		void openScene();
		void openScene(const std::filesystem::path& vScenePath);
		void saveScene();
		void saveSceneAs();
	private:
		// Some temporary panels
		bool m_IsShowDemoPanel = false;
		bool m_IsShowNewScenePanel = false;
		void showNewScenePanel(bool* vIsShow);
		bool m_IsShowRendererStatistics = true;
		void showRendererStatistics(bool* vIsShow);
		bool m_IsShowSceneRendererSettings = true;
		void showSceneRendererSettings(bool* vIsShow);
	private:
		// Shortcuts
		void initializeShortcutLibrary();          // 初始化快捷键  TODO : 有Project序列化后读取Project中包含快捷键设置
		// Event callbacks
		bool onKeyPressed(KeyPressedEvent& vEvent);
		bool onMousePressed(MouseButtonPressedEvent& vEvent);
	private:
		// Viewport
		Timestep m_LastFrameTime = 0.0f;
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
		Ref<SceneRenderer> m_Renderer = nullptr;
		// Scenes and SceneHierarchyPanel
		std::filesystem::path m_ActiveScenePath; // TODO : Delete! use project path
		Ref<Scene> m_ActiveScene = nullptr;
		std::vector<Ref<Scene>> m_Scenes; // TODO : Not use it yet !(only one FBO for now)
		// Panels
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel = nullptr;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel = nullptr;
		Scope<SceneStatePanel> m_SceneStatePanel = nullptr;
	};

}
