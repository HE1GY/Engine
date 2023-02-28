#pragma once

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

namespace Engine
{
	struct ScriptEngineData
	{
		MonoDomain* root_domain;
		MonoDomain* app_domain;
	};
	class ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();
	private:
		static ScriptEngineData* s_data;
	};

} // Engine
