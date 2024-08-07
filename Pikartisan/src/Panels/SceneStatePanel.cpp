#include "SceneStatePanel.h"

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

}