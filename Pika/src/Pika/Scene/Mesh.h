#pragma once
#include "Primitive.h"
#include "Materials.h"

namespace Pika {

	class StaticMesh {
	public:
		StaticMesh() = default;
		StaticMesh(const std::vector<StaticMeshVertexData>& vVertices,
			const std::vector<uint32_t>& vIndices, const Ref<Material>& vMaterial);

		inline const void* getVerticesData() const { return m_Vertices.data(); }
		inline uint32_t getVerticesSize() const { return static_cast<uint32_t>(m_Vertices.size()) * sizeof(StaticMeshVertexData); }
		inline const std::vector<StaticMeshVertexData>& getVertices() const { return m_Vertices; }
		inline const uint32_t* getIndicesData() const { return m_Indices.data(); }
		inline uint32_t getIndicesCount() const { return static_cast<uint32_t>(m_Indices.size()); }
		inline const std::vector<uint32_t>& getIndices() const { return m_Indices; }
	private:
		std::vector<StaticMeshVertexData> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Ref<Material> m_Material = nullptr;
	};

	class SkeletalMesh {
		// TODO !
	};

}
