#pragma once
#include "Pika/Core/KeyCodes.h"
#include "Pika/Core/MouseCodes.h"
#include <glm/glm.hpp>


namespace Pika {

	class PIKA_API Input
	{
	public:
		static bool isKeyPressed(const Key::KeyCode vKey);
		static bool isMouseButtonPressed(const Mouse::MouseCode vButton);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};

}
