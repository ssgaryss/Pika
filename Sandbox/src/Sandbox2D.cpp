#include "Sandbox2D.h"
#include <imgui/imgui.h>

void Sandbox2D::onAttach()
{
	PK_PROFILE_FUNCTION();
	m_TextureBackround = Pika::Texture2D::Create("assets/textures/board.png");
}

void Sandbox2D::onDetach()
{
	PK_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(Pika::Timestep vTimestep)
{
	PK_PROFILE_FUNCTION();

	m_CameraController.onUpdate(vTimestep);

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
