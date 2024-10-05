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

		inline const std::filesystem::path& getPath() const { return m_Path; }
		inline const std::vector<StaticMesh>& getMeshes() const { return m_Meshes; }
	private:
		void processNode(aiNode* vNode, const aiScene* vScene);
		StaticMesh processMesh(aiMesh* vMesh, const aiScene* vScene);
	private:
		bool m_IsLoaded = false;
		std::vector<StaticMesh> m_Meshes;   // TODO : Skeletal Mesh !
		std::filesystem::path m_Path;
	};


}
