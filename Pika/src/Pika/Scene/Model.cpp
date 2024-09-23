#include "pkpch.h"
#include "Model.h"
#include "Primitive.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Pika {

	namespace Utils {

		static bool IsModel(const std::filesystem::path& vPath) {
			return vPath.extension().string() == ".obj"
				|| vPath.extension().string() == ".fbx";
		}

	}

	Model::Model(const std::filesystem::path& vPath)
		: m_Path{ vPath }
	{
		try {
			loadModel(m_Path);
		}
		catch (const std::runtime_error& e) {
			PK_CORE_WARN(e.what());
		}
	}

	void Model::loadModel(const std::filesystem::path& vPath)
	{
		if (!Utils::IsModel(vPath))
			throw std::runtime_error(std::format(R"(Model : Fail to load the model with unsupported format at "{0}".)", vPath.string()));

		Assimp::Importer Importer;
		const aiScene* pScene = Importer.ReadFile(vPath.string(),
			aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace);

		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
			throw std::runtime_error(std::format(R"(Model : Fail to load the model at "{0}".)", vPath.string()) + "\nERROR::ASSIMP::" + Importer.GetErrorString());

		m_Path = vPath;
		processNode(pScene->mRootNode, pScene);
		PK_CORE_INFO("Model : Success to load a model at {0}.", vPath.string());
		m_IsLoaded = true;
	}

	void Model::processNode(aiNode* vNode, const aiScene* vScene)
	{
		// 处理节点的所有网格
		for (unsigned int i = 0; i < vNode->mNumMeshes; i++) {
			aiMesh* mesh = vScene->mMeshes[vNode->mMeshes[i]];
			m_Meshes.emplace_back(processMesh(mesh, vScene));
		}

		// 递归处理子节点
		for (unsigned int i = 0; i < vNode->mNumChildren; i++) {
			processNode(vNode->mChildren[i], vScene);
		}
	}

	StaticMesh Model::processMesh(aiMesh* vMesh, const aiScene* vScene)
	{
		std::vector<StaticMeshVertexData> Vertices;
		std::vector<uint32_t> Indices;

		// 遍历所有顶点
		for (unsigned int i = 0; i < vMesh->mNumVertices; i++) {
			StaticMeshVertexData Vertex;
			Vertex.m_Position = glm::vec3(vMesh->mVertices[i].x, vMesh->mVertices[i].y, vMesh->mVertices[i].z);
			Vertex.m_Normal = glm::vec3(vMesh->mNormals[i].x, vMesh->mNormals[i].y, vMesh->mNormals[i].z);

			if (vMesh->mTextureCoords[0]) {
				Vertex.m_TexCoords = glm::vec2(vMesh->mTextureCoords[0][i].x, vMesh->mTextureCoords[0][i].y);
			}
			else {
				Vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
			}

			//if (vMesh->HasTangentsAndBitangents()) {
			//	Vertex.m_Tangent = glm::vec3(vMesh->mTangents[i].x, vMesh->mTangents[i].y, vMesh->mTangents[i].z);
			//	Vertex.m_Bitangent = glm::vec3(vMesh->mBitangents[i].x, vMesh->mBitangents[i].y, vMesh->mBitangents[i].z);
			//}

			Vertices.emplace_back(Vertex);
		}

		// 遍历每个面的索引
		for (uint32_t i = 0; i < vMesh->mNumFaces; i++) {
			aiFace face = vMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++) {
				Indices.emplace_back(face.mIndices[j]);
			}
		}

		// 处理材质
		aiMaterial* material = vScene->mMaterials[vMesh->mMaterialIndex];
		//m_Textures = 
		// TODO !
		return StaticMesh(Vertices, Indices, nullptr);
	}

}