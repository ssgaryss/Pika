#pragma once
#include <Pika.h>

class Sandbox2D : public Pika::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;
	void onAttach() override;
	void onDetach() override;
	void onUpdate(Pika::Timestep vTimestep) override;
	void onImGuiRender() override;
	void onEvent(Pika::Event& vEvent) override;
private:
	Pika::Camera2DController m_CameraController;
	Pika::Ref<Pika::ShaderLibrary> m_ShaderLibrary;
	Pika::Ref<Pika::Texture> m_TextureBackround;

};

