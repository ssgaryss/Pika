#include "Pika.h"
#include "imgui/imgui.h"

class ExampleLayer : public Pika::Layer
{
public:
	ExampleLayer() 
		:Layer{ "Example Layer" } 
	{
		const char* Name = R"(Colored shader)";

		const char* VertexShader = R"(
		#version 460 core
		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec3 a_Color;

		out vec4 v_Color;

		uniform mat4 u_ViewProjectionMatrix;
			
		void main(){
			gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0f);
			v_Color = vec4(a_Color, 1.0f);
		})";

		const char* FragentShader = R"(
		out vec4 FragmentColor;

		in vec4 v_Color;
		
		void main(){
			FragmentColor = v_Color;
		})";

		const char* FragentShaderBlue = R"(
		out vec4 FragmentColor;

		in vec4 v_Color;
		
		void main(){
			FragmentColor = vec4(0.1f, 0.1f, 0.9f, 1.0f);
		})";

		shader_1 = Pika::Shader::Create(Name, VertexShader, FragentShader);
		shader_2 = Pika::Shader::Create(Name, VertexShader, FragentShaderBlue);

		VAO_1.reset(Pika::VertexArray::Create());
		VAO_1->bind();

		float vertices_pos[4 * 3]
		{
			 0.5f,  0.5f, 0.0f, // top right
			 0.5f, -0.5f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  // top left 
		};
		float vertices_color[4 * 3]
		{
			0.8f, 0.2f, 0.2f,
			0.2f, 0.8f, 0.2f,
			0.2f, 0.2f, 0.8f,
			0.5f, 0.5f, 0.5f
		};
		VBO_1.reset(Pika::VertexBuffer::Create(vertices_pos, sizeof(vertices_pos)));
		VBO_2.reset(Pika::VertexBuffer::Create(vertices_color, sizeof(vertices_color)));
		Pika::BufferLayout Layout_1 = {
			{Pika::ShaderDataType::Float3, "a_Position"}
		};
		Pika::BufferLayout Layout_2 = {
			{Pika::ShaderDataType::Float3, "a_Color"}
		};
		VBO_1->setLayout(Layout_1);
		VBO_2->setLayout(Layout_2);
		VAO_1->addVertexBuffer(VBO_1);
		VAO_1->addVertexBuffer(VBO_2);
		uint32_t indices[6]{
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle 
		};
		EBO.reset(Pika::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		VAO_1->setIndexBuffer(EBO);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		VBO_2->unbind();
		VBO_1->unbind();
		VAO_1->unbind();
		///////////////////////////////////////////////////////
		VAO_2.reset(Pika::VertexArray::Create());
		VAO_2->bind();
		float vertices[4 * 3]
		{
			 0.2f,  0.2f, 0.0f, // top right
			 0.2f, -0.2f, 0.0f, // bottom right
			-0.2f, -0.2f, 0.0f, // bottom left
			-0.2f,  0.2f, 0.0f,  // top left 
		};
		VBO_3.reset(Pika::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pika::BufferLayout Layout = {
			{Pika::ShaderDataType::Float3, "a_Position"}
		};
		VBO_3->setLayout(Layout);
		VAO_2->addVertexBuffer(VBO_3);
		VAO_2->setIndexBuffer(EBO);
		VAO_2->unbind();
	};

	void onUpdate() override{ 
		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::Tab)) {
			PK_TRACE("Tab is pressed!");
		}
		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::A)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(0.1f, 0.0f, 0.0f));
			PK_TRACE("Tab is pressed!");
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::D)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(-0.1f, 0.0f, 0.0f));
		}

		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::W)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(0.0f, 0.1f, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::S)) {
			Pika::Renderer::s_Camera->addPosition(glm::vec3(0.0f, -0.1f, 0.0f));
		}
		Pika::RenderCommand::SetClearColor(Pika::Color(0.1f, 0.1f, 0.1f, 1.0f));
		Pika::RenderCommand::Clear();
		Pika::Renderer::BeginScene();
		Pika::Renderer::Submit(shader_1.get(), VAO_1.get());
		Pika::Renderer::Submit(shader_2.get(), VAO_2.get());
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
		//ImGui::Begin("Settings");
		//ImGui::Text("Hello world!");
		//ImGui::End();
	}
	private:
		std::shared_ptr<Pika::VertexArray> VAO_1;
		std::shared_ptr<Pika::VertexArray> VAO_2;
		std::shared_ptr<Pika::VertexBuffer> VBO_1;
		std::shared_ptr<Pika::VertexBuffer> VBO_2;
		std::shared_ptr<Pika::VertexBuffer> VBO_3;
		std::shared_ptr<Pika::IndexBuffer> EBO;
		std::shared_ptr<Pika::Shader> shader_1;
		std::shared_ptr<Pika::Shader> shader_2;

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