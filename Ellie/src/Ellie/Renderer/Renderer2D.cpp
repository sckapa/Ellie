#include "eepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommands.h"

namespace Ellie {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage();

		// Geometry
		s_data->QuadVertexArray = Ellie::VertexArray::Create();

		float vertices[3 * 4]
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		auto layout = Ellie::BufferLayout({
			{Ellie::ShaderDataType::Float3, "a_Position"}
			});

		std::shared_ptr<Ellie::VertexBuffer> sqVB;
		sqVB.reset(Ellie::VertexBuffer::Create(vertices, sizeof(vertices)));

		sqVB->SetLayout(layout);
		s_data->QuadVertexArray->AddVertexBuffers(sqVB);

		uint32_t indices[6]{ 0,1,2,2,3,0 };

		std::shared_ptr<Ellie::IndexBuffer> sqIB;
		sqIB.reset(Ellie::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		s_data->QuadVertexArray->SetIndexBuffer(sqIB);

		// Shader
		s_data->Shader = Ellie::Shader::Create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->Shader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_data->Shader)->UploadUniformMat4("u_Transform", glm::mat4(1.0));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->Shader)->UploadUniformFloat4("u_Color", color);

		s_data->QuadVertexArray->Bind();
		RenderCommands::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color);
	}

}