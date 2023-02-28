#include "pch.h"

#include "config.h"
#include "ScriptEngine.h"
#include "Engine/Core/Core.h"

namespace Engine
{
	ScriptEngineData* ScriptEngine::s_data=nullptr;

	void ScriptEngine::Init()
	{
		s_data = new ScriptEngineData();

		mono_set_assemblies_path(CMAKE_SOURCE_DIR"/external/precompiled/lib/mono/4.5");

		s_data->root_domain = mono_jit_init("EngineScriptRuntime");
		if (s_data->root_domain == nullptr)
		{
			CORE_ASSERT(false, "Failed to init mono");
			return;
		}

		s_data->app_domain = mono_domain_create_appdomain((char*)"EngineAppDomain", nullptr);
		mono_domain_set(s_data->app_domain, true);
	}
	void ScriptEngine::ShutDown()
	{
		delete s_data->root_domain;
		delete s_data->app_domain;
		delete s_data;
	}
} // Engine