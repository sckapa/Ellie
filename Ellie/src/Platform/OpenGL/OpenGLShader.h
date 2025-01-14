#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Ellie/Renderer/Shader.h"

namespace Ellie {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string name, const int integer);

		void UploadUniformFloat(const std::string name, const float fl);
		void UploadUniformFloat2(const std::string name, const glm::vec2 float2);
		void UploadUniformFloat3(const std::string name, const glm::vec3 float3);
		void UploadUniformFloat4(const std::string name, const glm::vec4 float4);

		void UploadUniformMat3(const std::string name, const glm::mat3 matrix);
		void UploadUniformMat4(const std::string name, const glm::mat4 matrix);
	private:
		uint32_t m_RendererID;
	};
}

