#include "pkpch.h"
#include "SubTexture2D.h"

namespace Pika {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& vTexture, const glm::vec2& vLB, const glm::vec2& vTR)
		: m_Texture{ vTexture }
	{
		m_TextureCoordinates[0].x = vLB.x;
		m_TextureCoordinates[0].y = vLB.y; //LB
		m_TextureCoordinates[1].x = vTR.x;
		m_TextureCoordinates[1].y = vLB.y; //RB
		m_TextureCoordinates[2].x = vTR.x;
		m_TextureCoordinates[2].y = vTR.y; //TR
		m_TextureCoordinates[3].x = vLB.x;
		m_TextureCoordinates[3].y = vTR.y; //LT
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& vTexture, const glm::vec2& vCoordinates, const glm::vec2& vCellSize, const glm::vec2& vSpriteSize)
	{
		float Width = static_cast<float>(vTexture->getWidth());
		float Height = static_cast<float>(vTexture->getHeight());
		glm::vec2 LB{ (vCoordinates.x * vCellSize.x) / Width, (vCoordinates.y * vCellSize.y) / Height };
		glm::vec2 RT{ ((vCoordinates.x + vSpriteSize.x) * vCellSize.x) / Width, ((vCoordinates.y + vSpriteSize.y) * vCellSize.y) / Height };
		return CreateRef<SubTexture2D>(vTexture, LB, RT);
	}

}