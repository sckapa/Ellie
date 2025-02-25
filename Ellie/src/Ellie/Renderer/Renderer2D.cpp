#include "eepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "RenderCommands.h"

namespace Ellie {

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoords;
		float TexIndex;

		// Editor Only
		int EntityID;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexPtr = nullptr;

		uint32_t TexSlotIndex;
		std::array<Ref<Texture2D>, MaxTexSlots> TextureSlots;

		glm::vec4 QuadVerices[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DStorage s_data;

	void Renderer2D::Init()
	{
		// Geometry
		s_data.QuadVertexArray = Ellie::VertexArray::Create();

		s_data.QuadVertexBase = new QuadVertex[s_data.MaxVertices];

		auto layout = Ellie::BufferLayout({
			{Ellie::ShaderDataType::Float3, "a_Position"},
			{Ellie::ShaderDataType::Float4, "a_Color"},
			{Ellie::ShaderDataType::Float2, "a_TexCoords"},
			{Ellie::ShaderDataType::Float, "a_TexIndex"},
			{Ellie::ShaderDataType::Int, "a_EntityID"}
			});

		s_data.QuadVertexBuffer.reset(Ellie::VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex)));

		s_data.QuadVertexBuffer->SetLayout(layout);
		s_data.QuadVertexArray->AddVertexBuffers(s_data.QuadVertexBuffer);

		uint32_t* quadIndices = new uint32_t[s_data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<Ellie::IndexBuffer> sqIB;
		sqIB.reset(Ellie::IndexBuffer::Create(quadIndices, s_data.MaxIndices));
		s_data.QuadVertexArray->SetIndexBuffer(sqIB);

		delete[] quadIndices;

		// Shader
		int sampler[s_data.MaxTexSlots];
		for (uint32_t i = 0; i < s_data.MaxTexSlots; i++)
		{
			sampler[i] = i;
		}

		s_data.TextureShader = Ellie::Shader::Create("assets/shaders/Texture.glsl");
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetIntArray("u_Textures", sampler, s_data.MaxTexSlots);

		s_data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t texData = 0xffffffff;
		s_data.whiteTexture->SetData(&texData, sizeof(uint32_t));

		s_data.TextureSlots[0] = s_data.whiteTexture;

		s_data.QuadVerices[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_data.QuadVerices[1] = { 0.5f,-0.5f,0.0f,1.0f };
		s_data.QuadVerices[2] = { 0.5f,0.5f,0.0f,1.0f };
		s_data.QuadVerices[3] = { -0.5f,0.5f,0.0f,1.0f };
	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::BeginScene(const Camera& camera, glm::mat4 transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexPtr = s_data.QuadVertexBase;

		s_data.TexSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexPtr = s_data.QuadVertexBase;

		s_data.TexSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& editorCamera)
	{
		glm::mat4 viewProj = editorCamera.GetViewProjection();

		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexPtr = s_data.QuadVertexBase;

		s_data.TexSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_data.QuadVertexPtr - (uint8_t*)s_data.QuadVertexBase;
		s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBase, dataSize);
		Flush();
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexPtr = s_data.QuadVertexBase;

		s_data.TexSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_data.TexSlotIndex; i++)
		{
			s_data.TextureSlots[i]->Bind(i);
		}

		RenderCommands::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
		s_data.stats.DrawCount++;
	}

	//

	void Renderer2D::DrawQuad(const glm::vec2 position, const  glm::vec2 size, const  glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 position, const  glm::vec2 size, const  glm::vec4 color)
	{
		if (s_data.QuadIndexCount >= s_data.MaxIndices)
		{
			FlushAndReset();
		}

		const float texIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		glm::vec2 texCoords[] = { {0.0f,0.0f},{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_data.QuadVertexPtr->position = transform * s_data.QuadVerices[i];
			s_data.QuadVertexPtr->color = color;
			s_data.QuadVertexPtr->texCoords = texCoords[i];
			s_data.QuadVertexPtr->TexIndex = texIndex;
			s_data.QuadVertexPtr++;
		}

		s_data.QuadIndexCount += 6;

		s_data.stats.QuadCount++;
	}

	//

	void Renderer2D::DrawQuad(const glm::vec3 position, const  glm::vec2 size, const  Ref<Texture2D> texture)
	{
		if (s_data.QuadIndexCount >= s_data.MaxIndices)
		{
			FlushAndReset();
		}
		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TexSlotIndex; i++)
		{
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = s_data.TexSlotIndex;
			s_data.TextureSlots[s_data.TexSlotIndex] = texture;
			s_data.TexSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		glm::vec2 texCoords[] = { {0.0f,0.0f},{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_data.QuadVertexPtr->position = transform * s_data.QuadVerices[i];
			s_data.QuadVertexPtr->color = color;
			s_data.QuadVertexPtr->texCoords = texCoords[i];
			s_data.QuadVertexPtr->TexIndex = texIndex;
			s_data.QuadVertexPtr++;
		}

		s_data.QuadIndexCount += 6;

		s_data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2 position, const  glm::vec2 size, const  Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0 }, size, texture);
	}

	//

	void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4 color, int entityID)
	{
		if (s_data.QuadIndexCount >= s_data.MaxIndices)
		{
			FlushAndReset();
		}

		const float texIndex = 0.0f;

		glm::vec2 texCoords[] = { {0.0f,0.0f},{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_data.QuadVertexPtr->position = transform * s_data.QuadVerices[i];
			s_data.QuadVertexPtr->color = color;
			s_data.QuadVertexPtr->texCoords = texCoords[i];
			s_data.QuadVertexPtr->TexIndex = texIndex;
			s_data.QuadVertexPtr->EntityID = entityID;
			s_data.QuadVertexPtr++;
		}

		s_data.QuadIndexCount += 6;

		s_data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4 transform, const Ref<Texture2D> texture, const glm::vec4 color, int entityID)
	{

		if (s_data.QuadIndexCount >= s_data.MaxIndices)
		{
			FlushAndReset();
		}

		float texIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TexSlotIndex; i++)
		{
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = s_data.TexSlotIndex;
			s_data.TextureSlots[s_data.TexSlotIndex] = texture;
			s_data.TexSlotIndex++;
		}

		glm::vec2 texCoords[] = { {0.0f,0.0f},{ 1.0f,0.0f },{ 1.0f,1.0f },{ 0.0f,1.0f } };

		for (int i = 0; i < 4; i++)
		{
			s_data.QuadVertexPtr->position = transform * s_data.QuadVerices[i];
			s_data.QuadVertexPtr->color = color;
			s_data.QuadVertexPtr->texCoords = texCoords[i];
			s_data.QuadVertexPtr->TexIndex = texIndex;
			s_data.QuadVertexPtr->EntityID = entityID;
			s_data.QuadVertexPtr++;
		}

		s_data.QuadIndexCount += 6;

		s_data.stats.QuadCount++;
	}

	//

	void Renderer2D::DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0 }, size, rotationInRadians, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotationInRadians, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		
		DrawQuad(transform, color);
	}

	//

	void Renderer2D::DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotationInRadians, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		DrawQuad(transform, texture, glm::vec4(1.0f));
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0 }, size, rotationInRadians, texture);
	}

	//

	void Renderer2D::DrawSprite(const glm::mat4 transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
		{
			DrawQuad(transform, src.Texture, src.Color, entityID);
		}
		else
		{
			DrawQuad(transform, src.Color, entityID);
		}
	}

	//

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_data.stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data.stats, 0, sizeof(Statistics));
	}
}