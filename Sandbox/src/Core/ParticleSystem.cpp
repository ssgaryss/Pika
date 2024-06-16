#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(100);
	m_PoolIndex = m_ParticlePool.size() - 1;
}

void ParticleSystem::OnUpdate(Pika::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender()
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		Pika::Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, 0.9f }, { size ,size }, particle.Rotation, color);
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Pika::Random::GetFloat() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Pika::Random::GetFloat() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Pika::Random::GetFloat() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Pika::Random::GetFloat() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	if (m_PoolIndex > 0)
		m_PoolIndex--;
	else
		m_PoolIndex = m_ParticlePool.size() - 1;
}
