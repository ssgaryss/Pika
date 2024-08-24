#pragma once
#include "Mesh.h"
#include "Pika/Renderer/Texture.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Pika {


	class Model
	{
	public:
		Model() = default;
		Model(const std::filesystem::path& vPath);
		void loadModel(const std::filesystem::path& vPath);
	private:
		void processNode(aiNode* vNode, const aiScene* vScene);
		StaticMesh processMesh(aiMesh* vMesh, const aiScene* vScene);
	private:
		std::vector<StaticMesh> m_Meshes;   // TODO : Skeletal Mesh !
		std::vector<Ref<Texture2D>> m_Textures;
		std::filesystem::path m_Path;
	};

}
