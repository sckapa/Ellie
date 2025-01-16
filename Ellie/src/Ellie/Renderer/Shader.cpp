#include "eepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ellie {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(filepath);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
	{
		EE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Library[name] = shader;
	}

	void ShaderLibrary::Add(Ref<Shader> shader)
	{
		std::string name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(std::string name)
	{
		EE_CORE_ASSERT(Exists(name), "Shader does not exist!");
		return m_Library[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Library.find(name) != m_Library.end();
	}

}