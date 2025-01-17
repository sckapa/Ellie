#pragma once

#include "eepch.h"
#include "Ellie/Core/Core.h"
#include "Ellie/Events/Event.h"

namespace Ellie {

	struct WindowProps
	{
		std::string Title;
		unsigned int Height;
		unsigned int Width;

		WindowProps(const std::string title = "Ellie Engine", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Height(height), Width(width) { }
	};

	class ELLIE_API Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallBackFn& callack) = 0;
		virtual void SetVsync(const bool sync) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}