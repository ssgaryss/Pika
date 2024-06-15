#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

namespace Pika
{

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& vTexture, const glm::vec2& vLB, const glm::vec2& vTR);
		inline const Ref<Texture2D> getTexture() const { return m_Texture; }
		inline const std::array<glm::vec2, 4> getTextureCoordinates() const { return m_TextureCoordinates; }

		static Ref<SubTexture2D> Create(const Ref<Texture2D>& vTexture, const glm::vec2& vCoordinates,
			const glm::vec2& vCellSize, const glm::vec2& vSpriteSize);
	private:
		Ref<Texture2D> m_Texture;
		std::array<glm::vec2, 4> m_TextureCoordinates; // LB¡¢RB¡¢RT¡¢LT
	};

}
