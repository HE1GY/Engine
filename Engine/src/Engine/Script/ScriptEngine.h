#pragma once

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

namespace Engine
{
	enum class Accessibility : uint8_t
	{
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};
	struct ScriptEngineData
	{
		MonoDomain* root_domain = nullptr;
		MonoDomain* app_domain = nullptr;
	};
	class ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();
	private:
		static void InitMono();

		static ScriptEngineData* s_data;
	};

} // Engine
