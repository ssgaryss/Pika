#include "SceneStatePanel.h"
#include <imgui/imgui.h>

namespace Pika {

	SceneStatePanel::SceneStatePanel()
	{
		m_PlayButton = Texture2D::Create("resources/icons/SceneStatePanel/PlayButton.png");
		m_PauseButton = Texture2D::Create("resources/icons/SceneStatePanel/PauseButton.png");
		m_SimulateButton = Texture2D::Create("resources/icons/SceneStatePanel/SimulateButton.png");
		m_StopButton = Texture2D::Create("resources/icons/SceneStatePanel/StopButton.png");
		m_StepButton = Texture2D::Create("resources/icons/SceneStatePanel/StepButton.png");
	}

	SceneStatePanel::SceneStatePanel(const Ref<Scene>& vScene)
		: SceneStatePanel()
	{
		m_Context = vScene;
	}

	void SceneStatePanel::onImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));  // Padding = 2.0f
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 }); // 透明部分不显示
		ImGui::Begin("SceneStatePanel", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
		ImVec2 ContentRegionSize = ImGui::GetContentRegionAvail();
		ImVec2 CursorPosition = ImGui::GetCursorPos();
		float ButtonSize = ContentRegionSize.y - 4.0f; // 上下Padding均为2.0f
		ImVec2 ButtonsSize = { 4 * ButtonSize + 5 * 5.0f, ButtonSize };
		ImGui::SetCursorPos({ (ContentRegionSize.x - ButtonsSize.x) / 2.0f, ImGui::GetCursorPosY() });

		uintptr_t PlayButton = static_cast<uintptr_t>(m_PlayButton->getRendererID());
		uintptr_t PauseButton = static_cast<uintptr_t>(m_PauseButton->getRendererID());
		uintptr_t SimulateButton = static_cast<uintptr_t>(m_SimulateButton->getRendererID());
		uintptr_t StopButton = static_cast<uintptr_t>(m_StopButton->getRendererID());
		uintptr_t StepButton = static_cast<uintptr_t>(m_StepButton->getRendererID());
		bool HasPlayButton = getSceneState() == Scene::SceneState::Edit || (getSceneState() != Scene::SceneState::Edit && IsPaused());
		bool HasPauseButton = getSceneState() != Scene::SceneState::Edit && !IsPaused();
		bool HasSimulateButton = getSceneState() == Scene::SceneState::Edit || getSceneState() == Scene::SceneState::Simulate;
		bool HasStopButton = getSceneState() != Scene::SceneState::Edit;
		bool HasStepButton = getSceneState() == Scene::SceneState::Play || getSceneState() == Scene::SceneState::Simulate;

		if (HasPlayButton) {
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(PlayButton), { ButtonSize, ButtonSize }, { 0,1 }, { 1,0 })) {
				if (getSceneState() == Scene::SceneState::Edit) {
					setSceneState(Scene::SceneState::Play);
					m_Context->onRuntimeBegin();
				}
				else {
					m_Context->m_IsPaused = false;
				}
			}
			ImGui::SameLine();
		}
		if (HasPauseButton) {
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(PauseButton), { ButtonSize, ButtonSize }, { 0,1 }, { 1,0 })) {
				if (getSceneState() != Scene::SceneState::Edit) {
					m_Context->m_IsPaused = true;
				}
			}
			ImGui::SameLine();
		}
		if (HasStepButton)
		{
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(StepButton), { ButtonSize, ButtonSize }, { 0,1 }, { 1,0 })) {
				// TODO!
			}
			ImGui::SameLine();
		}if (HasSimulateButton) {
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(SimulateButton), { ButtonSize, ButtonSize }, { 0,1 }, { 1,0 })) {
				if (getSceneState() == Scene::SceneState::Edit) {
					setSceneState(Scene::SceneState::Simulate);
					m_Context->onSimulationBegin();
				}
			}
			ImGui::SameLine();
		}
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(StopButton), { ButtonSize, ButtonSize }, { 0,1 }, { 1,0 })) {
			if (getSceneState() == Scene::SceneState::Play) {
				m_Context->onRuntimeEnd();
			}
			if (getSceneState() == Scene::SceneState::Simulate) {
				m_Context->onSimulationEnd();
			}
			setSceneState(Scene::SceneState::Edit);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

}