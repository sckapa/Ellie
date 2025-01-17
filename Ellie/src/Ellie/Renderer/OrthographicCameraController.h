#pragma once

#include "Ellie/Renderer/OrthographicCamera.h"
#include "Ellie/Core/Timestep.h"
#include "Ellie/Events/ApplicationEvent.h"
#include "Ellie/Events/MouseEvent.h"

namespace Ellie {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera GetCamera() { return m_Camera; }
		const OrthographicCamera GetCamera() const { return m_Camera; }

	private:
		bool MouseScrolled(MouseScrolledEvent& e);
		bool WindowResize(WindowResizeEvent& e);

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_AllowRotation;

		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_Rotation = 0.0f;

		float m_CameraSpeed = 5.0f;
		float m_RotationSpeed = 180.0f;
	};

}

