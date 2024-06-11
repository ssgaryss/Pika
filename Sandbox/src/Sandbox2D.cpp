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
	Pika::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.9f }, { 10.0f, 10.0f }, m_TextureBackround, 10.0f);
	Pika::Renderer2D::drawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f });
	Pika::Renderer2D::drawRotatedQuad({ -0.5f, 0.5f }, { 0.5f, 0.5f }, glm::radians(Rotation), { 1.0f, 0.0f, 1.0f, 1.0f });
	Rotation += glm::radians(10.0f);
	Pika::Renderer2D::EndScene();

}

void Sandbox2D::onImGuiRender()
{
	PK_PROFILE_FUNCTION();

	ImGui::Begin("Color");
	ImGui::Text("hello!");
	ImGui::End();
}

void Sandbox2D::onEvent(Pika::Event& vEvent)
{
	PK_PROFILE_FUNCTION();
	m_CameraController.onEvent(vEvent);
}
