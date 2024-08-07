#pragma once

namespace Pika {

	class GraphicsContext
	{
	public:

		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;
		virtual void Initialize() = 0;
		virtual void swapBuffer() = 0;

		static Ref<GraphicsContext> Create(void* vWindow); //only GLFW for now
	};

}