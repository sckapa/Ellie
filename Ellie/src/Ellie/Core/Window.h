#pragma once

#include "eepch.h"
#include "Ellie/Core/Core.h"
#include "Ellie/Events/Event.h"

namespace Ellie {

	struct WindowProps
	{
		std::string Title;
		uint32_t Height;
		uint32_t Width;

		WindowProps(const std::string title = "Ellie Engine", uint32_t width = 1600, uint32_t height = 900)
			: Title(title), Height(height), Width(width) { }
	};

	class ELLIE_API Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallBackFn& callack) = 0;
		virtual void SetVsync(const bool sync) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}