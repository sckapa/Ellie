#pragma once

#include "Ellie/Core.h"

namespace Ellie{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};

}

