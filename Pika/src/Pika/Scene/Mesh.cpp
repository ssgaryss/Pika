#include "pkpch.h"
#include "Mesh.h"

namespace Pika {

	StaticMesh::StaticMesh(const std::vector<StaticMeshVertexData>& vVertices,
		const std::vector<uint32_t>& vIndices, const Ref<Material>& vMaterial)
		: m_Vertices{ vVertices }, m_Indices{ vIndices }, m_Material{ vMaterial }
	{
	}

}