#include "Sandbox2D.h"
#include <imgui/imgui.h>

Sandbox2D::Sandbox2D()
	: Layer{ "Sandbox2D" }, m_CameraController{ 1920.0f / 1080.0f },
	m_ShaderLibrary{ Pika::CreateRef<Pika::ShaderLibrary>() }
{
}

void Sandbox2D::onAttach()
{
	PK_PROFILE_FUNCTION();
	Pika::Renderer2D::Init();

	m_TextureBackround = Pika::Texture2D::Create("assets/textures/board.png");
	m_Texture2024 = Pika::Texture2D::Create("assets/textures/2024.png");
	//m_ShaderLibrary->load("assets/shaders/ShaderTexture.glsl");
}

void Sandbox2D::onDetach()
{
	PK_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Pika::Timestep vTimestep)
{
	PK_PROFILE_FUNCTION();

	m_CameraController.onUpdate(vTimestep);
	Pika::Renderer2D::BeginScene(m_CameraController);
	Pika::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.9f }, { 20.0f, 20.0f }, m_TextureBackround, 10.0f);
	Pika::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f });
	Pika::Renderer2D::DrawQuad({ -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	Pika::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f });
	Pika::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, glm::radians(Rotation), { 1.0f, 0.0f, 1.0f, 1.0f });
	Pika::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.1f }, { 0.5f, 0.5f }, glm::radians(Rotation), m_Texture2024);
	for (float x = -5.0f; x < 5.0f; x += 0.2f) {
		for (float y = -5.0f; y < 5.0f; y += 0.2f) {
			Pika::Renderer2D::DrawQuad({ x, y, -0.2f }, { 0.2f, 0.2f }, { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.5f, 1.0f });
		}
	}
	Rotation += glm::radians(10.0f);
	Pika::Renderer2D::EndScene();

}

void Sandbox2D::onImGuiRender()
{
	PK_PROFILE_FUNCTION();
	auto Statistics = Pika::Renderer2D::GetStatistics();
	ImGui::Begin("Renderer statistics");
	ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
	ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
	ImGui::End();
}

void Sandbox2D::onEvent(Pika::Event& vEvent)
{
	PK_PROFILE_FUNCTION();
	m_CameraController.onEvent(vEvent);
}
