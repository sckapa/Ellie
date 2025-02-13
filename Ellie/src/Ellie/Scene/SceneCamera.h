#pragma once

#include "Ellie/Renderer/Camera.h"

namespace Ellie {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		// Perspective

		float GetPerspectiveVerticalFOV() { return m_PerspectiveVerticalFOV; }
		void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; Recalculate(); }

		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; Recalculate(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; Recalculate(); }

		// Orthographic

		float GetOrthographicSize() { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; Recalculate(); }

		float GetOrthoNearClip() const { return m_OrthographicNear; }
		void SetOrthoNearClip(float nearClip) { m_OrthographicNear = nearClip; Recalculate(); }

		float GetOrthoFarClip() const { return m_OrthographicFar; }
		void SetOrthoFarClip(float FarClip) { m_OrthographicFar = FarClip; Recalculate(); }

		//

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		void Recalculate();

		float m_AspectRatio = 0.0f;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveVerticalFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
	};

}