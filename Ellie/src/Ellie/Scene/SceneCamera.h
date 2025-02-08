#pragma once

#include "Ellie/Renderer/Camera.h"

namespace Ellie {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; }

	private:
		void Recalculate();

		float m_AspectRatio = 0.0f;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
	};

}