#include "eepch.h"
#include "WindowsWindow.h"

namespace Ellie{
	//GLFWwindow* Ellie::WindowsWindow::m_window = nullptr;

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
		m_Context->SwapBuffers();
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

	void Ellie::WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_window);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void GLFWError(int error_code, const char* description)
	{
		EE_CORE_ERROR("GLFW Error ({0}) : {1}",error_code, description);
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
			glfwSetErrorCallback(GLFWError);
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)prop.Width, (int)prop.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = new OpenGLContext(m_window);

		m_Context->Init();

		glfwSetWindowUserPointer(m_window, &m_Data);
		SetVsync(false);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData data = *(WindowData*) glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallBack(event);
			});

		glfwSetWindowCloseCallback((m_window), [](GLFWwindow* window)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallBack(event);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS :
					{
						KeyPressedEvent event(key, 0);
						data.EventCallBack(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallBack(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallBack(event);
						break;
					}
				}
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				}
			});

		glfwSetScrollCallback((m_window), [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallBack(event);
		});

		glfwSetCursorPosCallback((m_window), [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallBack(event);
			});
	}

}
