#pragma once
#include "Camera.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "Pika/Scene/Mesh.h"
#include "Pika/Scene/Components.h"
#include "Pika/Scene/RenderDataExtractor.h"
#include "Pika/Scene/Materials.h"

namespace Pika {

	class Renderer3DDeffered
	{
	public:
		Renderer3DDeffered() = delete;

		static void BeginScene(const EditorCamera& vEditorCamera);
		static void BeginScene(const Camera& vCamera, const glm::mat4& vViewMatrix);
		static void EndScene();
		static void Flush();

		// Mesh
		static void DrawStaticMesh(const TransformComponent& vTransform, const StaticMesh& vMesh, int vEntityID = -1);
		static void DrawStaticMesh(const TransformComponent& vTransform, const StaticMesh& vMesh, const MaterialComponent& vMaterial, int vEntityID = -1);
		static void DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, int vEntityID = -1);
		static void DrawModel(const TransformComponent& vTransform, const ModelComponent& vModel, const MaterialComponent& vMaterial, int vEntityID = -1);
		// Line
		static void SetLineThickness(float vThickness);
		static float GetLineThickness();
		static void DrawLine(const glm::vec3& vStartPosition, const glm::vec3& vEndPosition, const glm::vec4& vColor = glm::vec4(1.0f));
		static void DrawGrid(const glm::mat4& vIdentityMatrix, float vSize, const glm::vec4& vColor = glm::vec4(1.0f), float vInterval = 1.0f); // 与2D不同,vIdentityMatrix默认为单位矩阵即在平面XOZ, vSize为中心到边的距离
		// Skybox
		static void DrawSkybox(const Ref<Cubemap>& vSkybox);
		// Shadow
		static void DrawVerticesPosition(const glm::mat4& vTransform, const StaticMesh& vMesh); // Shadow Map需要的场景数据
		static void DrawShadowMaps(const LightsData& vLightsData, const SceneData& vSceneData);

		struct Statistics {
			uint32_t m_DrawCalls = 0;
			uint32_t m_MeshCount = 0;
			uint32_t m_LineCount = 0;
			uint32_t getDrawCalls() const { return m_DrawCalls; }
			uint32_t getMeshCount() const { return m_MeshCount; }
			uint32_t getLineCount() const { return m_LineCount; }
		};

		static void ResetStatistics();
		static Statistics GetStatistics();
	private:
		static void StartBatch();
		static void NextBatch();
	};

}

