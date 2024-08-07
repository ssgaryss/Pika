#pragma once
#include <Pika.h>

namespace Pika {

	class SceneStatePanel
	{
	public:
		SceneStatePanel();
		SceneStatePanel(const Ref<Scene>& vScene);

		inline void setContext(Ref<Scene> vScene) { m_Context = vScene; }
		inline Scene::SceneState getSceneState() const { return m_Context->m_SceneState; }
		inline void setSceneState(Scene::SceneState vSceneState) { m_Context->m_SceneState = vSceneState; }
	private:
		Ref<Scene> m_Context = nullptr;
	private:
		Ref<Texture2D> m_PlayButton;
		Ref<Texture2D> m_PauseButton;
		Ref<Texture2D> m_SimulateButton;
		Ref<Texture2D> m_StopButton;
		Ref<Texture2D> m_StepButton;  // TODO : Not using it !
	};

}
