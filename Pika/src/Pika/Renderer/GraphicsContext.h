#pragma once

namespace Pika {

	class GraphicsContext
	{
	public:
		struct GraphicsContextInformation
		{
			std::string m_Vendor;
			std::string m_Renderer;
			std::string m_Version;
			GraphicsContextInformation() = default;
		};
	public:

		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;
		virtual void Initialize() = 0;
		virtual void swapBuffer() = 0;

		inline const GraphicsContextInformation* getInformation() const { return &m_Information; }

		static Ref<GraphicsContext> Create(void* vWindow); //only GLFW for now
	protected:
		GraphicsContextInformation m_Information;
	};

}