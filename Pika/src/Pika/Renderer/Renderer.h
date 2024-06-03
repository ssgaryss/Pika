#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Pika
{

	class Renderer
	{
	public:
		static void BeginScene();
		static void Submit(const Shader* vShader, const VertexArray* vData, const glm::mat4 vTransform = glm::mat4(1.0f)); //submit Scene info
		static void EndScene();
		static Scope<Camera> s_Camera;
	private:
		struct SceneDate
		{
			glm::mat4 ViewProjectionMatrix;
		};
		SceneDate m_SceneData;
	};

}
