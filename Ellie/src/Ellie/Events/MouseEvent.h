#pragma once

#include "Event.h"

namespace Ellie {

	class ELLIE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		inline float GetX() { return m_MouseX; }
		inline float GetY() { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent : " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	//

	class ELLIE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y) : m_MouseOffsetX(x), m_MouseOffsetY(y) {}

		inline float GetOffsetX() { return m_MouseOffsetX; }
		inline float GetOffsetY() { return m_MouseOffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent : " << m_MouseOffsetX << ", " << m_MouseOffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseOffsetX, m_MouseOffsetY;
	};

	//

	class ELLIE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent : " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) : m_Button(button){}
		int m_Button;
	};

	//

	class ELLIE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button){ }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent : " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	//

	class ELLIE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) { }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent : " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}