#include "Pika.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Pika::Layer
{
public:
	ExampleLayer()
		:Layer{ "Example Layer" }
	{
		const char* VertexShader = R"(
		#version 460 core
		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec2 a_TexCoord;
		out vec2 v_TexCoord;

		uniform mat4 u_ViewProjectionMatrix;
		uniform mat4 u_Transform;
			
		void main(){
			gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f);
			v_TexCoord = a_TexCoord;
		})";

		const char* FragentShader = R"(
		layout (location = 0) out vec4 FragmentColor;
		in vec2 v_TexCoord;
		uniform sampler2D u_Texture0;
		
		void main(){
			FragmentColor = texture(u_Texture0, v_TexCoord);
		})";

		const char* FragentShaderBlue = R"(
		layout (location = 0) out vec4 FragmentColor;
		
		uniform vec3 u_Color;
	
		void main(){
			FragmentColor = vec4(u_Color, 1.0f);
		})";


		Pika::Ref<Pika::Shader> shader_1 = m_ShaderLibrary.load("shader_1", "assets/shaders/ShaderTexture.glsl");
		Pika::Ref<Pika::Shader> shader_2 = Pika::Shader::Create("shader_2", VertexShader, FragentShaderBlue);
		m_ShaderLibrary.add(shader_2);

		VAO_1 = Pika::VertexArray::Create();
		VAO_1->bind();

		float vertices_pos[4 * 5]
		{
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,// top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left 
		};
		VBO_1 = Pika::VertexBuffer::Create(vertices_pos, sizeof(vertices_pos));
		Pika::BufferLayout Layout_1 = {
			{Pika::ShaderDataType::Float3, "a_Position"},
			{Pika::ShaderDataType::Float2, "a_TexCoord"}
		};
		VBO_1->setLayout(Layout_1);
		VAO_1->addVertexBuffer(VBO_1);
		uint32_t indices[6]{
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle 
		};
		EBO = Pika::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		VAO_1->setIndexBuffer(EBO);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		VBO_1->unbind();
		VAO_1->unbind();
		///////////////////////////////////////////////////////
		VAO_2 = (Pika::VertexArray::Create());
		VAO_2->bind();
		float vertices[4 * 3]
		{
			 0.2f,  0.2f, 0.0f, // top right
			 0.2f, -0.2f, 0.0f, // bottom right
			-0.2f, -0.2f, 0.0f, // bottom left
			-0.2f,  0.2f, 0.0f,  // top left 
		};
		VBO_3 = (Pika::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pika::BufferLayout Layout = {
			{Pika::ShaderDataType::Float3, "a_Position"}
		};
		VBO_3->setLayout(Layout);
		VAO_2->addVertexBuffer(VBO_3);
		VAO_2->setIndexBuffer(EBO);
		VAO_2->unbind();

		texture1 = Pika::Texture2D::Create(R"(assets/textures/2024.png)");
		//PK_INFO(" u_Texture0 width = {0}, height = {1}", texture1->getWidth(), texture1->getHeight());
		shader_1->bind();
		shader_1->setInt("u_Texture0", 0);
	};

	void onUpdate(Pika::Timestep vTimestep) override {
		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::A)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(-1.0f * vTimestep, 0.0f, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::D)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(1.0f * vTimestep, 0.0f, 0.0f));
		}

		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::W)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(0.0f, 1.0f * vTimestep, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::S)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(0.0f, -1.0f * vTimestep, 0.0f));
		}
		Pika::RenderCommand::SetClearColor(Pika::Color(0.1f, 0.1f, 0.1f, 1.0f));
		Pika::RenderCommand::Clear();
		Pika::Renderer::BeginScene();
		texture1->bind();
		Pika::Renderer::Submit(m_ShaderLibrary.getShader("shader_1").get(), VAO_1.get(), glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.2f, 0.0f)));
		Pika::Ref<Pika::Shader> shader_2 = m_ShaderLibrary.getShader("shader_2"); 
		shader_2->bind();
		shader_2->setFloat3("u_Color", m_Color);
		float stride = 0.2f;
		glm::mat4 Transform(1.0f);
		Transform = glm::translate(Transform, glm::vec3(-0.2f, 0.2f, 0.0f));
		Transform = glm::scale(Transform, glm::vec3(0.1f));
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				Pika::Renderer::Submit(shader_2.get(), VAO_2.get(), glm::translate(Transform, glm::vec3(0.5f * i, -0.5f * j, 0.0f)));
			}
		}
		Pika::Renderer::EndScene();
	}

	void onEvent(Pika::Event& vEvent) override {
		//PK_TRACE(vEvent.toString());
		if (vEvent.getEventType() == Pika::EventType::KeyPressed) {
			Pika::KeyPressedEvent& Event = static_cast<Pika::KeyPressedEvent&>(vEvent);
			PK_TRACE("Event Layer:Key {0} is pressed!", (char)Event.getKeyCode());
		}
	}
	void onImGuiRender() override {
		ImGui::Begin("Color Board");
		ImGui::ColorEdit3("Color Editor", glm::value_ptr(m_Color));
		ImGui::End();
	}
private:
	Pika::ShaderLibrary m_ShaderLibrary;
	Pika::Ref<Pika::VertexArray> VAO_1;
	Pika::Ref<Pika::VertexArray> VAO_2;
	Pika::Ref<Pika::VertexBuffer> VBO_1;
	Pika::Ref<Pika::VertexBuffer> VBO_2; // no use for now
	Pika::Ref<Pika::VertexBuffer> VBO_3;
	Pika::Ref<Pika::IndexBuffer> EBO;
	Pika::Ref<Pika::Texture2D> texture1;
	glm::vec3 m_Color = glm::vec3(0.1f, 0.1f, 0.8f);

};

class Sandbox : public Pika::Application
{
public:
	Sandbox()
		:Application{}
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Pika::ImGuiLayer());
	};
	~Sandbox() {};
};

Pika::Application* Pika::createApplication() {
	return new Sandbox();
}