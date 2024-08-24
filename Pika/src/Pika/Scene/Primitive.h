#pragma once
#include <glm/glm.hpp>


namespace Pika {

	struct QuadVertexData {
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec4 m_Color = glm::vec4(1.0f);
		glm::vec2 m_TexCoord = glm::vec2(0.0f);
		int m_TextureIndex = 0;
		float m_TilingFactor = 1.0f;
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
		glm::vec3 m_Position;                   // 顶点位置 (x, y, z)
		glm::vec3 m_Normal;                     // 顶点法线
		glm::vec2 m_TexCoords;                  // 纹理坐标 (u, v)
		glm::vec3 m_Tangent;                    // 切线向量 (可选，用于法线贴图)
		glm::vec3 m_Bitangent;                  // 副切线向量 (可选，用于法线贴图)
		StaticMeshVertexData() = default;
	};

	struct SkeletalMeshVertexData
	{
		// TODO !
	};

}