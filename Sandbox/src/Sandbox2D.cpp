#include "Sandbox2D.h"
#include <imgui/imgui.h>

static const char* s_Map =
"000000000000000000000000000000"
"000000000000000000000000000000"
"000000000111111111111111110000"
"000000111111111111111111100000"
"000001111111100111111111000000"
"000001111111000011111110000000"
"000001111111000011111110000000"
"000000111111111111111111000000"
"000000001111111111111000000000"
"000000000000000000000000000000"
"000000000000000000000000000000"
"000000000000000000000000000000"
"000000000000000000000000000000"
"000000011111111111111000000000"
"000000111111111111111100000000"
"000000000111111111100000000000"
"000000011111111000000000000000"
"000000000111000000111100000000"
"000000000000000000011000000000"
"000000000000000000000000000000";

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
	m_TextureRPGpack_sheet_2X = Pika::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_TextureTree = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 2, 1 }, { 1, 2 }, { 128, 128 });
	m_TextureWater = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 11, 11 }, { 1, 1 }, { 128, 128 });
	m_TextureGround = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 1, 11 }, { 1, 1 }, { 128, 128 });
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
	int num = 0;
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 30; ++j) {
			if (s_Map[i * 30 + j] == '0')
				Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureWater);
			else if (s_Map[i * 30 + j] == '1')
				Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureGround);
			else
				Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureTree);
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
