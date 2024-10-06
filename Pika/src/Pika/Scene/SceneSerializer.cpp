#include "pkpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>


// TODO : yaml对于glm数据类型输入流运算符重载，暂时放这
namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, const glm::vec2& vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, const glm::vec3& vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y << vData.z;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

	YAML::Emitter& operator<<(YAML::Emitter& vOstream, const glm::vec4& vData) {
		vOstream << YAML::Flow << YAML::BeginSeq;
		vOstream << vData.x << vData.y << vData.z << vData.w;
		vOstream << YAML::EndSeq;
		return vOstream;
	}

}


namespace Pika {

	namespace Utils {

		static std::string CameraProjectionModeToString(Camera::CameraProjectionMode vMode) {
			using namespace std::string_literals;
			switch (vMode)
			{
			case Pika::Camera::CameraProjectionMode::Othographic:
				return "Othographic"s;
			case Pika::Camera::CameraProjectionMode::Perspective:
				return "Perspective"s;
			}
			PK_CORE_WARN("SceneSerializer : Unknown Camera Projection Mode !");
			return ""s;
		}

		static Camera::CameraProjectionMode StringToCameraProjectionMode(const std::string& vString) {
			using namespace std::string_literals;
			if (vString == "Othographic"s) return Camera::CameraProjectionMode::Othographic;
			if (vString == "Perspective"s) return Camera::CameraProjectionMode::Perspective;
			PK_CORE_WARN("SceneSerializer : Unknown Camera Projection Mode !");
			return Camera::CameraProjectionMode::Othographic;
		}

		static std::string RigidbodyTypeToString(Rigidbody2DComponent::RigidbodyType vType) {
			using namespace std::string_literals;
			switch (vType)
			{
			case Pika::Rigidbody2DComponent::RigidbodyType::Static:
				return "Static"s;
			case Pika::Rigidbody2DComponent::RigidbodyType::Kinematic:
				return "Kinematic"s;
			case Pika::Rigidbody2DComponent::RigidbodyType::Dynamic:
				return "Dynamic"s;
			}
			PK_CORE_WARN("SceneSerializer : Unknown Rigidbody Type Mode !");
			return ""s;
		}

		static Rigidbody2DComponent::RigidbodyType StringToRigidbodyType(const std::string& vString) {
			using namespace std::string_literals;
			if (vString == "Static"s) return Rigidbody2DComponent::RigidbodyType::Static;
			if (vString == "Kinematic"s) return Rigidbody2DComponent::RigidbodyType::Kinematic;
			if (vString == "Dynamic"s) return Rigidbody2DComponent::RigidbodyType::Dynamic;
			PK_CORE_WARN("SceneSerializer : Unknown Camera Projection Mode !");
			return Rigidbody2DComponent::RigidbodyType::Static;
		}

		static std::string SceneTypeToString(Scene::SceneType vType) {
			using namespace std::string_literals;
			switch (vType)
			{
			case Pika::Scene::SceneType::Scene2D:
				return "Scene2D"s;
			case Pika::Scene::SceneType::Scene3D:
				return "Scene3D"s;
			}
			PK_CORE_WARN("SceneSerializer : Unknown Scene Type !");
			return ""s;
		}

		static Scene::SceneType StringToSceneType(const std::string& vString) {
			using namespace std::string_literals;
			if (vString == "Scene2D"s) return Scene::SceneType::Scene2D;
			if (vString == "Scene3D"s) return Scene::SceneType::Scene3D;
			PK_CORE_WARN("SceneSerializer : Unknown Scene Type !");
			return Scene::SceneType::Scene2D;
		}

	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& vScene)
		: m_Scene{ vScene } {}

	void SceneSerializer::serializeYAMLText(const std::string& vFilePath)
	{
		std::string SceneName = m_Scene->getSceneName();
		YAML::Emitter Out;

		Out << YAML::BeginMap; // 每个Scene
		{
			Out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap;
			{
				Out << YAML::Key << "Name" << YAML::Value << SceneName;
				Out << YAML::Key << "SceneType" << YAML::Value << Utils::SceneTypeToString(m_Scene->getSceneType());
				Out << YAML::Key << "Skybox" << YAML::Value << (m_Scene->m_Skybox ? m_Scene->m_Skybox->getPath().string() : "None");
				Out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // 所有Entities
				{
					m_Scene->m_Registry.view<IDComponent>().each([&Out, this](auto vEntity, auto& vTagComponent) {
						Entity Entity{ vEntity, m_Scene.get() };
						Out << YAML::BeginMap; // 每个Entity
						if (Entity.hasComponent<IDComponent>()) {
							Out << YAML::Key << "Entity" << YAML::Value << Entity.getComponent<IDComponent>().m_ID;
						}
						if (Entity.hasComponent<TagComponent>()) {
							Out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Tag" << YAML::Value << Entity.getComponent<TagComponent>().m_Tag;
							}
							Out << YAML::EndMap;
						}
						if (Entity.hasComponent<TransformComponent>()) {
							auto& Transform = Entity.getComponent<TransformComponent>();
							Out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Position" << YAML::Value << Transform.m_Position;
								Out << YAML::Key << "Rotation" << YAML::Value << Transform.m_Rotation;
								Out << YAML::Key << "Scale" << YAML::Value << Transform.m_Scale;
							}
							Out << YAML::EndMap;
						}
						if (Entity.hasComponent<CameraComponent>()) {
							Out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
							{
								auto& Camera = Entity.getComponent<CameraComponent>();
								Out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
								{
									Out << YAML::Key << "ProjectionMode" << YAML::Value << Utils::CameraProjectionModeToString(Camera.m_Camera.getProjectionMode());
									Out << YAML::Key << "OthographicSize" << YAML::Value << Camera.m_Camera.getOthographicSize();
									Out << YAML::Key << "OthographicNear" << YAML::Value << Camera.m_Camera.getOthographicNear();
									Out << YAML::Key << "OthographicFar" << YAML::Value << Camera.m_Camera.getOthographicFar();
									Out << YAML::Key << "PerspectiveFOV" << YAML::Value << Camera.m_Camera.getPerspectiveFOV();
									Out << YAML::Key << "PerspectiveNear" << YAML::Value << Camera.m_Camera.getPerspectiveNear();
									Out << YAML::Key << "PerspectiveFar" << YAML::Value << Camera.m_Camera.getPerspectiveFar();
									Out << YAML::Key << "AspectRatio" << YAML::Value << Camera.m_Camera.getAspectRatio();
								}
								Out << YAML::EndMap;
								Out << YAML::Key << "IsFixedAspectRatio" << YAML::Value << Camera.m_IsFixedAspectRatio;
							}
							Out << YAML::EndMap;
						}
						// 3D Components
						if (Entity.hasComponent<ModelComponent>()) {
							auto& Model = Entity.getComponent<ModelComponent>();
							Out << YAML::Key << "ModelComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Path" << YAML::Value << (Model.m_Model ? Model.m_Model->getPath().string() : "None");
							}
							Out << YAML::EndMap;
						}

						if (Entity.hasComponent<MaterialComponent>()) {
							auto& Material = Entity.getComponent<MaterialComponent>();
							Out << YAML::Key << "MaterialComponent" << YAML::Value << YAML::BeginMap;
							{
								if (Material.m_Material) {
									std::string Type = Material.m_Material->getType();
									Out << YAML::Key << "Type" << YAML::Value << Type;
									if (auto pBlinnPhoneMaterial = dynamic_cast<BlinnPhoneMaterial*>(Material.m_Material.get())) {
										Out << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
										{
											const auto& Data = pBlinnPhoneMaterial->getData();
											Out << YAML::Key << "Ambient" << YAML::Value << Data.m_Ambient;
											Out << YAML::Key << "Diffuse" << YAML::Value << Data.m_Diffuse;
											Out << YAML::Key << "Specular" << YAML::Value << Data.m_Specular;
											Out << YAML::Key << "Shininess" << YAML::Value << Data.m_Shininess;
											Out << YAML::Key << "Diffuse Map" << YAML::Value << (Data.m_DiffuseMap ? Data.m_DiffuseMap->getPath().string() : "None");
											Out << YAML::Key << "Specular Map" << YAML::Value << (Data.m_SpecularMap ? Data.m_DiffuseMap->getPath().string() : "None");
										}
										Out << YAML::EndMap;
									}
								}
								else {
									std::string Type = "None";
									Out << YAML::Key << "Type" << YAML::Value << Type;
								}
							}
							Out << YAML::EndMap;
						}

						if (Entity.hasComponent<LightComponent>()) {
							auto& Light = Entity.getComponent<LightComponent>();
							Out << YAML::Key << "LightComponent" << YAML::Value << YAML::BeginMap;
							{
								if (Light.m_Light) {
									std::string Type = Light.m_Light->getType();
									Out << YAML::Key << "Type" << YAML::Value << Type;
									if (auto pDirectionLight = dynamic_cast<DirectionLight*>(Light.m_Light.get())) {
										Out << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
										{
											const auto& Data = pDirectionLight->getData();
											Out << YAML::Key << "Light Color" << YAML::Value << Data.m_LightColor;
											Out << YAML::Key << "Intensity" << YAML::Value << Data.m_Intensity;
											Out << YAML::Key << "Enable Shadow" << YAML::Value << Data.m_EnableShadow;
											Out << YAML::Key << "Light Size" << YAML::Value << Data.m_LightSize;
										}
										Out << YAML::EndMap;
									}
									else if (auto pPointLight = dynamic_cast<PointLight*>(Light.m_Light.get())) {
										Out << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
										{
											const auto& Data = pPointLight->getData();
											Out << YAML::Key << "Light Color" << YAML::Value << Data.m_LightColor;
											Out << YAML::Key << "Intensity" << YAML::Value << Data.m_Intensity;
											Out << YAML::Key << "Constant" << YAML::Value << Data.m_Constant;
											Out << YAML::Key << "Linear" << YAML::Value << Data.m_Linear;
											Out << YAML::Key << "Quadratic" << YAML::Value << Data.m_Quadratic;
											Out << YAML::Key << "Enable Shadow" << YAML::Value << Data.m_EnableShadow;
											Out << YAML::Key << "Light Size" << YAML::Value << Data.m_LightSize;
										}
										Out << YAML::EndMap;
									}
								}
								else {
									std::string Type = "None";
									Out << YAML::Key << "Type" << YAML::Value << Type;
								}
							}
							Out << YAML::EndMap;
						}
						// 2D Components
						if (Entity.hasComponent<SpriteRendererComponent>()) {
							auto& SpriteRenderer = Entity.getComponent<SpriteRendererComponent>();
							Out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
							{
								Out << YAML::Key << "Color" << YAML::Value << SpriteRenderer.m_Color;
								Out << YAML::Key << "Texture" << YAML::Value << (SpriteRenderer.m_Texture ? SpriteRenderer.m_Texture->getPath().string() : "None");
								Out << YAML::Key << "Tiling Factor" << YAML::Value << SpriteRenderer.m_TilingFactor;
							}
							Out << YAML::EndMap;
						}
						if (Entity.hasComponent<Rigidbody2DComponent>()) {
							Out << YAML::Key << "Rigidbody2DComponent" << YAML::Value << YAML::BeginMap;
							{
								auto& Rigidbody2D = Entity.getComponent<Rigidbody2DComponent>();
								Out << YAML::Key << "RigidbodyType" << YAML::Value << Utils::RigidbodyTypeToString(Rigidbody2D.m_Type);
								Out << YAML::Key << "IsFixedRotation" << YAML::Value << Rigidbody2D.m_IsFixedRotation;
							}
							Out << YAML::EndMap;
						}
						if (Entity.hasComponent<BoxCollider2DComponent>()) {
							Out << YAML::Key << "BoxCollider2DComponent" << YAML::Value << YAML::BeginMap;
							{
								auto& BoxCollider2D = Entity.getComponent<BoxCollider2DComponent>();
								Out << YAML::Key << "Offset" << YAML::Value << BoxCollider2D.m_Offset;
								Out << YAML::Key << "Size" << YAML::Value << BoxCollider2D.m_Size;
								Out << YAML::Key << "Density" << YAML::Value << BoxCollider2D.m_Density;
								Out << YAML::Key << "Friction" << YAML::Value << BoxCollider2D.m_Friction;
								Out << YAML::Key << "Restitution" << YAML::Value << BoxCollider2D.m_Restitution;
								Out << YAML::Key << "RestitutionThreshold" << YAML::Value << BoxCollider2D.m_RestitutionThreshold;
							}
							Out << YAML::EndMap;
						}

						Out << YAML::EndMap;
						});
				}
				Out << YAML::EndSeq;
			}
			Out << YAML::EndMap;
		}
		Out << YAML::EndMap;

		// Write in file
		std::ofstream FileOut(vFilePath, std::ios_base::out | std::ios_base::trunc);
		PK_CORE_TRACE(R"(SceneSerializer : Try to serialize .pika file at "{0}" ...)", vFilePath);
		if (!FileOut.is_open()) {
			PK_CORE_ERROR(R"(SceneSerializer : Fail to open yaml file at "{0}".)", vFilePath);
			return;
		}
		FileOut << Out.c_str();
		PK_CORE_INFO(R"(SceneSerializer : Success to save scene "{0}".)", SceneName);
	}

	void SceneSerializer::serializeYAMLBinary(const std::string& vFilePath)
	{
		//TODO
	}

	bool SceneSerializer::deserializeYAMLText(const std::string& vFilePath)
	{
		YAML::Node Data;
		try
		{
			Data = YAML::LoadFile(vFilePath);
		}
		catch (const YAML::ParserException& e)
		{
			PK_CORE_ERROR(R"(SceneSerializer : Failed to load .pika file at "{0}".)", vFilePath);
			std::cerr << e.what() << std::endl;
			return false;
		}

		if (!Data["Scene"]) {
			PK_CORE_ERROR(R"(SceneSerializer : Not a valid scene yaml file "{0}")", vFilePath);
			return false;
		}

		PK_CORE_TRACE(R"(SceneSerializer : Try to deserialize .pika file at "{0}" ...)", vFilePath);
		auto SceneNode = Data["Scene"];
		// Name
		std::string SceneName = SceneNode["Name"].as<std::string>();
		m_Scene->setSceneName(SceneName);
		m_Scene->setSceneType(Utils::StringToSceneType(SceneNode["SceneType"].as<std::string>()));
		std::string SkyboxPath = SceneNode["Skybox"].as<std::string>();
		if (SkyboxPath != "None")
			m_Scene->setSkybox(Cubemap::Create(SkyboxPath));
		// Entities
		YAML::Node Entities = SceneNode["Entities"];
		if (Entities) {
			for (auto Entity : Entities) {
				auto UUIDString = Entity["Entity"].as<std::string>();
				std::string EntityName;
				if (Entity["TagComponent"]) {
					auto TagComponentNode = Entity["TagComponent"];
					EntityName = TagComponentNode["Tag"].as<std::string>();
				}
				auto DeserializedEntity = m_Scene->createEntityWithUUIDString(UUIDString, EntityName);
				PK_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", UUIDString, EntityName);

				if (Entity["TransformComponent"]) {
					auto TransformComponentNode = Entity["TransformComponent"];
					auto& TC = DeserializedEntity.getComponent<TransformComponent>();
					TC.m_Position = TransformComponentNode["Position"].as<glm::vec3>();
					TC.m_Rotation = TransformComponentNode["Rotation"].as<glm::vec3>();
					TC.m_Scale = TransformComponentNode["Scale"].as<glm::vec3>();
				}

				if (Entity["CameraComponent"]) {
					auto CameraComponentNode = Entity["CameraComponent"];
					auto CameraNode = CameraComponentNode["Camera"];
					auto& CC = DeserializedEntity.addComponent<CameraComponent>();
					CC.m_Camera.setProjectionMode(Utils::StringToCameraProjectionMode(CameraNode["ProjectionMode"].as<std::string>()));
					CC.m_Camera.setOthographicSize(CameraNode["OthographicSize"].as<float>());
					CC.m_Camera.setOthographicNear(CameraNode["OthographicNear"].as<float>());
					CC.m_Camera.setOthographicFar(CameraNode["OthographicFar"].as<float>());
					CC.m_Camera.setPerspectiveFOV(CameraNode["PerspectiveFOV"].as<float>());
					CC.m_Camera.setPerspectiveNear(CameraNode["PerspectiveNear"].as<float>());
					CC.m_Camera.setPerspectiveFar(CameraNode["PerspectiveFar"].as<float>());
					CC.m_Camera.setAspectRatio(CameraNode["AspectRatio"].as<float>());
					CC.m_IsFixedAspectRatio = CameraComponentNode["IsFixedAspectRatio"].as<bool>();
				}
				// 3D Components
				if (Entity["ModelComponent"]) {
					auto ModelComponentNode = Entity["ModelComponent"];
					auto& MC = DeserializedEntity.addComponent<ModelComponent>();
					std::string ModelPath = ModelComponentNode["Path"].as<std::string>();
					MC.m_Model = ModelPath == "None" ? nullptr : CreateRef<Model>(std::filesystem::path(ModelPath));
				}

				if (Entity["MaterialComponent"]) {
					auto MaterialComponentNode = Entity["MaterialComponent"];
					auto& MC = DeserializedEntity.addComponent<MaterialComponent>();
					std::string Type = MaterialComponentNode["Type"].as<std::string>();
					if (Type != "None") {
						if (Type == "Blinn-Phone") {
							auto DataNode = MaterialComponentNode["Data"];
							BlinnPhoneMaterial::Data Data;
							Data.m_Ambient = DataNode["Ambient"].as<glm::vec3>();
							Data.m_Diffuse = DataNode["Diffuse"].as<glm::vec3>();
							Data.m_Specular = DataNode["Specular"].as<glm::vec3>();
							Data.m_Shininess = DataNode["Shininess"].as<float>();
							std::string DiffuseMapPath = DataNode["Diffuse Map"].as<std::string>();
							Data.m_DiffuseMap = DiffuseMapPath == "None" ? nullptr : Texture2D::Create(std::filesystem::path(DiffuseMapPath));
							std::string SpecularMapPath = DataNode["Specular Map"].as<std::string>();
							Data.m_SpecularMap = SpecularMapPath == "None" ? nullptr : Texture2D::Create(std::filesystem::path(SpecularMapPath));
							MC.m_Material = CreateRef<BlinnPhoneMaterial>(Data);
						}
					}
					else {
						MC.m_Material = nullptr;
					}
				}

				if (Entity["LightComponent"]) {
					auto LightComponentNode = Entity["LightComponent"];
					auto& LC = DeserializedEntity.addComponent<LightComponent>();
					std::string Type = LightComponentNode["Type"].as<std::string>();
					if (Type != "None") {
						if (Type == "Direction Light") {
							auto DataNode = LightComponentNode["Data"];
							DirectionLight::Data Data;
							Data.m_LightColor = DataNode["Light Color"].as<glm::vec3>();
							Data.m_Intensity = DataNode["Intensity"].as<float>();
							Data.m_EnableShadow = DataNode["Enable Shadow"].as<bool>();
							Data.m_LightSize = DataNode["Light Size"].as<float>();
							LC.m_Light = CreateRef<DirectionLight>(Data);
						}
						else if (Type == "Point Light") {
							auto DataNode = LightComponentNode["Data"];
							PointLight::Data Data;
							Data.m_LightColor = DataNode["Light Color"].as<glm::vec3>();
							Data.m_Intensity = DataNode["Intensity"].as<float>();
							Data.m_Constant = DataNode["Constant"].as<float>();
							Data.m_Linear = DataNode["Linear"].as<float>();
							Data.m_Quadratic = DataNode["Quadratic"].as<float>();
							Data.m_EnableShadow = DataNode["Enable Shadow"].as<bool>();
							Data.m_LightSize = DataNode["Light Size"].as<float>();
							LC.m_Light = CreateRef<PointLight>(Data);
						}
					}
					else {
						LC.m_Light = nullptr;
					}
				}

				// 2D Components
				if (Entity["SpriteRendererComponent"]) {
					auto SpriteRendererComponentNode = Entity["SpriteRendererComponent"];
					auto& SRC = DeserializedEntity.addComponent<SpriteRendererComponent>();
					SRC.m_Color = SpriteRendererComponentNode["Color"].as<glm::vec4>();
					std::string TexturePath = SpriteRendererComponentNode["Texture"].as<std::string>();
					SRC.m_Texture = TexturePath == "None" ? nullptr : Texture2D::Create(std::filesystem::path(TexturePath));
					SRC.m_TilingFactor = SpriteRendererComponentNode["Tiling Factor"].as<glm::vec2>();
				}

				if (Entity["Rigidbody2DComponent"]) {
					auto Rigidbody2DComponentNode = Entity["Rigidbody2DComponent"];
					auto& R2DC = DeserializedEntity.addComponent<Rigidbody2DComponent>();
					R2DC.m_Type = Utils::StringToRigidbodyType(Rigidbody2DComponentNode["RigidbodyType"].as<std::string>());
					R2DC.m_IsFixedRotation = Rigidbody2DComponentNode["IsFixedRotation"].as<bool>();
				}

				if (Entity["BoxCollider2DComponent"]) {
					auto BoxCollider2DComponentNode = Entity["BoxCollider2DComponent"];
					auto& BC2DC = DeserializedEntity.addComponent<BoxCollider2DComponent>();
					BC2DC.m_Offset = BoxCollider2DComponentNode["Offset"].as<glm::vec2>();
					BC2DC.m_Size = BoxCollider2DComponentNode["Size"].as<glm::vec2>();
					BC2DC.m_Density = BoxCollider2DComponentNode["Density"].as<float>();
					BC2DC.m_Friction = BoxCollider2DComponentNode["Friction"].as<float>();
					BC2DC.m_Restitution = BoxCollider2DComponentNode["Restitution"].as<float>();
					BC2DC.m_RestitutionThreshold = BoxCollider2DComponentNode["RestitutionThreshold"].as<float>();
				}
			}
		}

		PK_CORE_INFO(R"(SceneSerializer : Success to load scene "{0}".)", SceneName);
		return true;
	}

	bool SceneSerializer::deserializeYAMLBinary(const std::string& vFilePath)
	{
		// TODO
		return false;
	}

}