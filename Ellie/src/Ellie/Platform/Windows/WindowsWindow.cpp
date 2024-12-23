#include "eepch.h"
#include "WindowsWindow.h"


namespace Ellie{

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& prop)
	{
		return new WindowsWindow(prop);
	}

	Ellie::WindowsWindow::WindowsWindow(const WindowProps& prop)
	{
		Init(prop);
	}

	void Ellie::WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Ellie::WindowsWindow::SetVsync(const bool sync)
	{
		if (sync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = sync;
	}

	bool Ellie::WindowsWindow::IsVsync() const
	{
		return m_Data.VSync;
	}

	void Ellie::WindowsWindow::Init(const WindowProps& prop)
	{
		m_Data.Title = prop.Title;
		m_Data.Height = prop.Height;
		m_Data.Width = prop.Width;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			EE_ASSERT(success, "Could not initialize GLFW!")
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)prop.Width, (int)prop.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_Data);
		SetVsync(true);
	}

	void Ellie::WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_window);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}
}
