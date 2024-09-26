#pragma once
#include "Camera.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "Pika/Scene/Mesh.h"
#include "Pika/Scene/Components.h"
#include "Pika/Scene/RenderDataExtractor.h"
#include "Pika/Scene/Materials.h"

namespace Pika
{

	class Renderer3D
	{
	public:
		Renderer3D() = delete;
		static void Initialize();
		//static void BeginScene(const EditorCamera& vEditorCamera, const LightsData& vLights);
		static void BeginScene(const EditorCamera& vEditorCamera, const LightsData& vLightsData);
		static void BeginScene(const Camera& vCamera, const glm::mat4& vTramsform); // TODO : Lights
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
		static void DrawGrid(const glm::mat4& vIdentityMatrix, float vSize, const glm::vec4& vColor = glm::vec4(1.0f), float vInterval = 1.0f); // ��2D��ͬ,vIdentityMatrixĬ��Ϊ��λ������ƽ��XOZ, vSizeΪ���ĵ��ߵľ���
		// Skybox
		static void RenderSkybox(const Ref<Cubemap>& vSkybox);

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
