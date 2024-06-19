#pragma once
#include <Pika.h>
#include "Core/ParticleSystem.h"

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
	bool onMouseMovedEvent(Pika::MouseMovedEvent& vEvent);
private:
	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
	Pika::Camera2DController m_CameraController;
	Pika::Ref<Pika::ShaderLibrary> m_ShaderLibrary;
	Pika::Ref<Pika::Texture2D> m_TextureBackround;
	Pika::Ref<Pika::Texture2D> m_Texture2024;
	Pika::Ref<Pika::Texture2D> m_TextureRPGpack_sheet_2X;
	Pika::Ref<Pika::SubTexture2D> m_TextureTree;
	Pika::Ref<Pika::SubTexture2D> m_TextureWater, m_TextureGround;

	float Rotation = 0.0f; //for now

};

