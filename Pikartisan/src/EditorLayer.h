#pragma once
#include <Pika.h>
#include "Panels/SceneHierarchyPanel.h"

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
		// Viewport
		bool m_IsViewportFocus = false;
		bool m_IsViewportHovered = false;
	private:
		// Renderer
		Camera2DController m_CameraController;
		Ref<ShaderLibrary> m_ShaderLibrary;
		Ref<Framebuffer> m_Framebuffer;
		// Scene
		Ref<Scene> m_ActiveScene;
		Entity m_BulueQuad;
		Entity m_RedQuad;
		// Panels
		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel; // TODO : Mutiple Scenes

		Ref<Texture2D> m_TextureBackround;
		Ref<Texture2D> m_Texture2024;
		Ref<Texture2D> m_TextureRPGpack_sheet_2X;
		Ref<SubTexture2D> m_TextureTree;
		Ref<SubTexture2D> m_TextureWater, m_TextureGround;

		float Rotation = 0.0f; //for now

	};

}
