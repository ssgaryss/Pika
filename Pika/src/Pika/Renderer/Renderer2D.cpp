#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include <glm/glm.hpp>


namespace Pika {

	static struct Renderer2DData {
		struct Camera2DData {
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		};
		Camera2DData m_Camera2DData;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		RenderCommand::Init();
	}

	void Renderer2D::BeginScene(Camera2DController& vCameraController)
	{
		s_Data.m_Camera2DData.m_ViewProjectionMatrix = vCameraController.getCamera().getViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

}