#pragma once

namespace Pika {

	class GraphicsContext
	{
	public:

		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;
		virtual void init() = 0;
		virtual void swapBuffer() = 0;

		static std::shared_ptr<GraphicsContext> createContext(void* vWindow); //only GLFW for now
	private:

	};

}