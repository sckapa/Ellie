#include "eepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommands.h"

namespace Ellie {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage();

		// Geometry
		s_data->QuadVertexArray = Ellie::VertexArray::Create();

		float vertices[5 * 4]
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		auto layout = Ellie::BufferLayout({
			{Ellie::ShaderDataType::Float3, "a_Position"},
			{Ellie::ShaderDataType::Float2, "a_TexCoords"}
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
		s_data->TextureShader = Ellie::Shader::Create("assets/shaders/Texture.glsl");
		s_data->TextureShader->Bind();
		s_data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::ShutDown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data->TextureShader->Bind();
		s_data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		s_data->TextureShader->Bind();
		s_data->TextureShader->SetFloat4("u_Color", color);

		Ref<Texture2D> whiteTexture = Texture2D::Create(1, 1);
		uint32_t texData = 0xffffffff;
		whiteTexture->SetData(&texData);

		whiteTexture->Bind();
		s_data->TextureShader->SetInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->TextureShader->SetMat4("u_Transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommands::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		s_data->TextureShader->Bind();
		s_data->TextureShader->SetFloat4("u_Color", {1.0f,1.0f,1.0f,1.0f});
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->TextureShader->SetMat4("u_Transform", transform);

		s_data->QuadVertexArray->Bind();
		RenderCommands::DrawIndexed(s_data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, texture);
	}

}