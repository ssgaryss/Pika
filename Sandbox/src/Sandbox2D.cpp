#include "Sandbox2D.h"
#include <imgui/imgui.h>

void Sandbox2D::onAttach()
{
	m_TextureBackround = Pika::Texture2D::Create("assets/textures/board.png");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Pika::Timestep vTimestep)
{
	m_CameraController.onUpdate(vTimestep);

}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Color");
	ImGui::Text("hello!");
	ImGui::End();
}

void Sandbox2D::onEvent(Pika::Event& vEvent)
{
	m_CameraController.onEvent(vEvent);
}
