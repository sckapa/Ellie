#include "eepch.h"
#include "OrthographicCameraController.h"

#include "Ellie/Core/Input.h"
#include "Ellie/Core/KeyCodes.h"

namespace Ellie {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_AllowRotation(rotation), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Ellie::Input::IsKeyPressed(EE_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		else if (Ellie::Input::IsKeyPressed(EE_KEY_A))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}

		if (Ellie::Input::IsKeyPressed(EE_KEY_S))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		else if (Ellie::Input::IsKeyPressed(EE_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}

		if(m_AllowRotation)
		{
			if (Ellie::Input::IsKeyPressed(EE_KEY_Q))
			{
				m_Rotation += m_RotationSpeed * ts;
			}
			else if (Ellie::Input::IsKeyPressed(EE_KEY_E))
			{
				m_Rotation -= m_RotationSpeed * ts;
			}
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_Rotation);

		m_CameraSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EE_BIND_EVENT_FN(OrthographicCameraController::MouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EE_BIND_EVENT_FN(OrthographicCameraController::WindowResize));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::MouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::WindowResize(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}