#include "pch.h"

#include "config.h"
#include "ScriptEngine.h"
#include "Engine/Core/Core.h"
#include <mono/metadata/attrdefs.h>

namespace Engine
{
	template<typename T>
	struct ScriptField
	{
		Accessibility access{ Accessibility::None };
		T value;
		MonoClassField* mono_field = nullptr;
	};

	static char* ReadBytes(const std::string& file_path, uint32_t* out_size)
	{
		std::ifstream stream(file_path, std::ios::binary | std::ios::ate);

		CORE_ASSERT(stream, "Failed to open file {0}", file_path);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		CORE_ASSERT(size, "File {0}  is empty!", file_path);

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*out_size = size;
		return buffer;
	}

	static MonoAssembly* LoadCSharpAssembly(const std::string& assembly_path)
	{
		uint32_t file_size = 0;
		char* file_data = ReadBytes(assembly_path, &file_size);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(file_data, file_size, 1, &status, 0);
		CORE_ASSERT((status == MONO_IMAGE_OK), "Mono ERROR: {0}", mono_image_strerror(status));

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assembly_path.c_str(), &status, 0);
		CORE_ASSERT(assembly, "Mono failed to load assembly!");
		mono_image_close(image);

		delete[] file_data;

		return assembly;
	}

	static void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* type_definitions_table = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t types_count = mono_table_info_get_rows(type_definitions_table);

		INFO("Assembly Info:");
		for (int32_t i = 0; i < types_count; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(type_definitions_table, i, cols, MONO_TYPEDEF_SIZE);

			const char* name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			INFO("Namespace {0}", name_space);
			INFO("Type name {0}", name);
		}
	}

	static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);
		CORE_ASSERT(klass, "Mono: failed to load class !");

		return klass;
	}

	static MonoObject* GetInstance(MonoDomain* domain, MonoClass* klass)
	{
		MonoObject* class_instance = mono_object_new(domain, klass);
		CORE_ASSERT(class_instance, "Failed to create instance of class");
		mono_runtime_object_init(class_instance);

		return class_instance;
	}

	template<typename... Args>
	static void CallMethod(MonoClass* klass, MonoObject* instance, const std::string& method_name, Args... args)
	{
		constexpr size_t param_count = sizeof...(args);

		MonoMethod* method = mono_class_get_method_from_name(klass, method_name.c_str(), param_count);
		CORE_ASSERT(method, "There is not method_name with name = {0}", method_name);

		std::array<void*, param_count> params;
		if (param_count)
		{
			size_t index = 0;
			auto AddToArray = [&](auto& arg)
			{
			  params[index++] = &arg;
			};

			(AddToArray(args), ...);
		}

		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, instance, params.data(), &exception);

		CORE_ASSERT((exception == nullptr), "Unhandled exception from script", method_name);
	}

	static uint8_t GetFieldAccessibility(MonoClassField* field)
	{
		uint8_t accessibility = (uint8_t)Accessibility::None;
		uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

		switch (accessFlag)
		{
		case MONO_FIELD_ATTR_PRIVATE:
		{
			accessibility = (uint8_t)Accessibility::Private;
			break;
		}
		case MONO_FIELD_ATTR_FAM_AND_ASSEM:
		{
			accessibility |= (uint8_t)Accessibility::Protected;
			accessibility |= (uint8_t)Accessibility::Internal;
			break;
		}
		case MONO_FIELD_ATTR_ASSEMBLY:
		{
			accessibility = (uint8_t)Accessibility::Internal;
			break;
		}
		case MONO_FIELD_ATTR_FAMILY:
		{
			accessibility = (uint8_t)Accessibility::Protected;
			break;
		}
		case MONO_FIELD_ATTR_FAM_OR_ASSEM:
		{
			accessibility |= (uint8_t)Accessibility::Private;
			accessibility |= (uint8_t)Accessibility::Protected;
			break;
		}
		case MONO_FIELD_ATTR_PUBLIC:
		{
			accessibility = (uint8_t)Accessibility::Public;
			break;
		}
		}

		return accessibility;
	}

	template<typename T>
	static ScriptField<T> GetFiled(MonoClass* klass, MonoObject* instance, const std::string& filed_name)
	{
		MonoClassField* mono_field = mono_class_get_field_from_name(klass, filed_name.c_str());
		uint8_t floatFieldAccessibility = GetFieldAccessibility(mono_field);

		float value;
		mono_field_get_value(instance, mono_field, &value);

		ScriptField<T> field{ (Accessibility)floatFieldAccessibility, value, mono_field };
		return field;
	}

	template<typename T>
	static void SetFiled(MonoObject* instance, ScriptField<T> field, T value)
	{
		mono_field_set_value(instance, field.mono_field, &value);
	}

	static uint8_t GetPropertyAccessibility(MonoProperty* property)
	{
		uint8_t accessibility = (uint8_t)Accessibility::None;

		// Get a reference to the property's getter method
		MonoMethod* propertyGetter = mono_property_get_get_method(property);
		if (propertyGetter != nullptr)
		{
			// Extract the access flags from the getters flags
			uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

			switch (accessFlag)
			{
			case MONO_FIELD_ATTR_PRIVATE:
			{
				accessibility = (uint8_t)Accessibility::Private;
				break;
			}
			case MONO_FIELD_ATTR_FAM_AND_ASSEM:
			{
				accessibility |= (uint8_t)Accessibility::Protected;
				accessibility |= (uint8_t)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_ASSEMBLY:
			{
				accessibility = (uint8_t)Accessibility::Internal;
				break;
			}
			case MONO_FIELD_ATTR_FAMILY:
			{
				accessibility = (uint8_t)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_FAM_OR_ASSEM:
			{
				accessibility |= (uint8_t)Accessibility::Private;
				accessibility |= (uint8_t)Accessibility::Protected;
				break;
			}
			case MONO_FIELD_ATTR_PUBLIC:
			{
				accessibility = (uint8_t)Accessibility::Public;
				break;
			}
			}
		}

		// Get a reference to the property's setter method
		MonoMethod* propertySetter = mono_property_get_set_method(property);
		if (propertySetter != nullptr)
		{
			// Extract the access flags from the setters flags
			uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
			if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
				accessibility = (uint8_t)Accessibility::Private;
		}
		else
		{
			accessibility = (uint8_t)Accessibility::Private;
		}

		return accessibility;
	}

	static bool CheckMonoError(MonoError& error)
	{
		bool hasError = !mono_error_ok(&error);
		if (hasError)
		{
			unsigned short errorCode = mono_error_get_error_code(&error);
			const char* errorMessage = mono_error_get_message(&error);
			printf("Mono Error!\n");
			printf("\tError Code: %hu\n", errorCode);
			printf("\tError Message: %s\n", errorMessage);
			mono_error_cleanup(&error);
		}
		return hasError;
	}

	static std::string MonoStringToUTF8(MonoString* monoString)
	{
		if (monoString == nullptr || mono_string_length(monoString) == 0)
			return "";

		MonoError error;
		char* utf8 = mono_string_to_utf8_checked(monoString, &error);
		//char* utf8 = mono_string_to_utf8(monoString);
		if (CheckMonoError(error))
			return "";
		std::string result(utf8);
		mono_free(utf8);
		return result;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path(CMAKE_SOURCE_DIR"/external/precompiled/lib/mono/4.5");

		s_data->root_domain = mono_jit_init("EngineScriptRuntime");
		CORE_ASSERT(s_data->root_domain, "Failed to init mono");

		s_data->app_domain = mono_domain_create_appdomain((char*)"EngineAppDomain", nullptr);
		mono_domain_set(s_data->app_domain, true);
	}

	ScriptEngineData* ScriptEngine::s_data = nullptr;

	void ScriptEngine::Init()
	{
		s_data = new ScriptEngineData();

		InitMono();

		MonoAssembly* assembly = LoadCSharpAssembly(CMAKE_SOURCE_DIR "/ScriptingSandbox/bin/ScriptingSandbox.dll");

		PrintAssemblyTypes(assembly);

		MonoClass* CSharpTesting_class = GetClassInAssembly(assembly, "ScriptingSandbox", "CSharpTesting");

		MonoObject* CSharpTesting_class_instance = GetInstance(s_data->app_domain, CSharpTesting_class);

		CallMethod(CSharpTesting_class, CSharpTesting_class_instance, "PrintFloatVar");

		CallMethod(CSharpTesting_class, CSharpTesting_class_instance, "IncrementFloatVar", 1.0f);

		CallMethod(CSharpTesting_class, CSharpTesting_class_instance, "PrintFloatVar");

		auto MyPublicFloatVar = GetFiled<float>(CSharpTesting_class, CSharpTesting_class_instance, "MyPublicFloatVar");

		if ((uint8_t)MyPublicFloatVar.access & (uint8_t)Accessibility::Public)
		{
			SetFiled<float>(CSharpTesting_class_instance, MyPublicFloatVar, -1.0f);
		}

		CallMethod(CSharpTesting_class, CSharpTesting_class_instance, "PrintFloatVar");

		MonoProperty* nameProperty = mono_class_get_property_from_name(CSharpTesting_class, "Name");
		uint8_t namePropertyAccessibility = GetPropertyAccessibility(nameProperty);

		if (namePropertyAccessibility & (uint8_t)Accessibility::Public)
		{
			MonoString* nameValue = (MonoString*)mono_property_get_value(nameProperty, CSharpTesting_class_instance,
					nullptr,
					nullptr);
			std::string nameStr = MonoStringToUTF8(nameValue);

			// Modify and assign the value back to the property by invoking the setter method
			nameStr += ", World!";
			nameValue = mono_string_new(s_data->app_domain, nameStr.c_str());
			mono_property_set_value(nameProperty, CSharpTesting_class_instance, (void**)&nameValue, nullptr);
		}

		delete assembly;
	}

	void ScriptEngine::ShutDown()
	{
		//mono_domain_unload(s_data->app_domain);
		s_data->app_domain = nullptr;

		//mono_jit_cleanup(s_data->root_domain);
		s_data->root_domain = nullptr;

		delete s_data;
	}
} // Engine