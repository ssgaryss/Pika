#include "pkpch.h"
#include "SubTexture2D.h"

namespace Pika {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& vTexture, const glm::vec2& vLB, const glm::vec2& vTR)
		: m_Texture{ vTexture }
	{
		float Width = static_cast<float>(vTexture->getWidth());
		float Height = static_cast<float>(vTexture->getHeight());
		m_TextureCoordinates[0].x = std::clamp(vLB.x, 0.0f, 1.0f);
		m_TextureCoordinates[0].y = std::clamp(vLB.y, 0.0f, 1.0f); //LB
		m_TextureCoordinates[1].x = std::clamp(vTR.x, 0.0f, 1.0f);
		m_TextureCoordinates[1].y = std::clamp(vLB.y, 0.0f, 1.0f); //RB
		m_TextureCoordinates[2].x = std::clamp(vTR.x, 0.0f, 1.0f);
		m_TextureCoordinates[2].y = std::clamp(vTR.y, 0.0f, 1.0f); //TR
		m_TextureCoordinates[3].x = std::clamp(vLB.x, 0.0f, 1.0f);
		m_TextureCoordinates[3].y = std::clamp(vTR.y, 0.0f, 1.0f); //LT
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& vTexture, const glm::vec2& vCoordinates, const glm::vec2& vSpriteSize, const glm::vec2& vCellSize)
	{
		float Width = static_cast<float>(vTexture->getWidth());
		float Height = static_cast<float>(vTexture->getHeight());
		glm::vec2 LB{ (vCoordinates.x * vCellSize.x) / Width, (vCoordinates.y * vCellSize.y) / Height };
		glm::vec2 RT{ ((vCoordinates.x + vSpriteSize.x) * vCellSize.x) / Width, ((vCoordinates.y + vSpriteSize.y) * vCellSize.y) / Height };
		return CreateRef<SubTexture2D>(vTexture, LB, RT);
	}

}