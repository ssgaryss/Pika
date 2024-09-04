#pragma once
#include "Primitive.h"
#include "Material.h"

namespace Pika {

	class StaticMesh {
	public:
		StaticMesh() = default;
		StaticMesh(const std::vector<StaticMeshVertexData>& vVertices,
			const std::vector<uint32_t>& vIndices, const Ref<Material>& vMaterial);

		const std::vector<StaticMeshVertexData>& getVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& getIndices() const { return m_Indices; }
	private:
		std::vector<StaticMeshVertexData> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Ref<Material> m_Material = nullptr;
	};

	class SkeletalMesh {
		// TODO !
	};

}
