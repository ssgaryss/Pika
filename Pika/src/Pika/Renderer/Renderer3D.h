#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera2D.h"

namespace Pika
{

	//TODO : Renderer3D need to be refactored!!!
	class Renderer3D
	{
	public:
		Renderer3D() = delete;
		static void Initialize();
		static void BeginScene();
		static void Submit(const Shader* vShader, const VertexArray* vData, const glm::mat4 vTransform = glm::mat4(1.0f)); //submit Scene info
		static void EndScene();

		//TODO : !!!!
		static Scope<Camera2D> s_Camera;
	private:
		struct SceneDate
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Scope<SceneDate> m_SceneData;
	};

}
