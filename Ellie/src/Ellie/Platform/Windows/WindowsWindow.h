#pragma once

#include "Ellie/Window.h"
#include <GLFW/glfw3.h>
#include "Ellie/Events/ApplicationEvent.h"
#include "Ellie/Events/KeyEvent.h"
#include "Ellie/Events/MouseEvent.h"
#include "GLFW/glfw3.h"

namespace Ellie{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& prop);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		virtual void* GetNativeWindow() const override { return m_window; }

		inline void SetEventCallback(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVsync(const bool sync) override;
		bool IsVsync() const override;

	private:
		virtual void Init(const WindowProps& prop);
		virtual void ShutDown();

		GLFWwindow* m_window;

		struct WindowData
		{
			std::string Title;
			unsigned int Height, Width;
			bool VSync;

			EventCallBackFn EventCallBack;
		};

		WindowData m_Data;
	};
}

