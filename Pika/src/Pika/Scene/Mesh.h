#pragma once
#include "Primitive.h"
#include "Material.h"

namespace Pika {

	class StaticMesh {
	public:
		StaticMesh() = default;
	private:
		std::vector<StaticMeshVertexData> m_Vertices;
		std::vector<uint32_t> m_Indices;
		Ref<Material> m_Material = nullptr;
	};

	class SkeletalMesh {
		// TODO !
	};

}
