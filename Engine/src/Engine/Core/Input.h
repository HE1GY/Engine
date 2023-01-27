#pragma once

namespace Engine
{
	class Input
	{
	public:
		static bool IsMouseButtonPress(int button);

		static bool IsKeyPress(int key_code);

		static std::pair<float, float> GetMousePos();
	};
} // namespace Engine