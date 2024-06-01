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
		static void Submit(const Shader* vShader, const VertexArray* vData); //submit Scene info
		static void EndScene();
		static Scope<Camera> s_Camera;
	private:
	};

}
