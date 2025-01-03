#pragma once
#include <glm/glm.hpp>


namespace Pika {

	struct QuadVertexData {
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);
		glm::vec2 m_TexCoord = glm::vec2(0.0f);
		int m_TextureIndex = 0;
		glm::vec2 m_TilingFactor = glm::vec2(1.0f);
		// TODO : Editor only
		int m_EntityID = -1;
	};

	struct LineVertexData
	{
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);

		int m_EntityID = -1;
	};

	struct StaticMeshVertexData
	{
		glm::vec3 m_Position = glm::vec3(0.0f);      // 顶点位置 (x, y, z)
		glm::vec3 m_Normal = glm::vec3(0.0f);        // 顶点法线
		glm::vec2 m_TexCoords = glm::vec2(0.0f);     // 纹理坐标 (u, v)

		int m_EntityID = -1;
		StaticMeshVertexData() = default;
	};

	struct SkeletalMeshVertexData
	{
		// TODO !
	};

}