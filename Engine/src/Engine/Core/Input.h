#pragma once

#include "KeyCode.h"
#include "MouseButtonCode.h"
namespace Engine
{
	class Input
	{
	public:
		static bool IsMouseButtonPress(int button);
		static bool IsMouseButtonPress(MouseButtonCode button);

		static bool IsKeyPress(int key_code);
		static bool IsKeyPress(KeyCode key_code);

		static std::pair<float, float> GetMousePos();
	};
} // namespace Engine