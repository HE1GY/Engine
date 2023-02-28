#include "pch.h"

#include "config.h"
#include "ScriptEngine.h"
#include "Engine/Core/Core.h"
//#include <mono/metadata/attrdefs.h>

namespace Engine
{
	static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			CORE_ASSERT(false, "Failed to open file {0}",filepath);
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			CORE_ASSERT(false, "File {0}  is empty!", filepath);
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}


	static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			CORE_ASSERT(false, "Mono ERROR: {0}", errorMessage);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		if (assembly == nullptr)
		{
			CORE_ASSERT(false, "Mono failed to load assembly!");
			return nullptr;
		}

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	static void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			printf("%s.%s\n", nameSpace, name);
		}
	}

	static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		if (klass == nullptr)
		{
			CORE_ASSERT(false, "Mono: failed to load class !");
			return nullptr;
		}

		return klass;
	}


	static enum class Accessibility : uint8_t
	{
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};

	/*static uint8_t GetFieldAccessibility(MonoClassField* field)
	{
		uint8_t accessibility = (uint8_t)Accessibility::None;
		uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

		switch (accessFlag)
		{
		case MONO_FIELD_ATTR_PRIVATE: {
			accessibility = (uint8_t)Accessibility::Private;
			break;
		}
		case MONO_FIELD_ATTR_FAM_AND_ASSEM: {
			accessibility |= (uint8_t)Accessibility::Protected;
			accessibility |= (uint8_t)Accessibility::Internal;
			break;
		}
		case MONO_FIELD_ATTR_ASSEMBLY: {
			accessibility = (uint8_t)Accessibility::Internal;
			break;
		}
		case MONO_FIELD_ATTR_FAMILY: {
			accessibility = (uint8_t)Accessibility::Protected;
			break;
		}
		case MONO_FIELD_ATTR_FAM_OR_ASSEM: {
			accessibility |= (uint8_t)Accessibility::Private;
			accessibility |= (uint8_t)Accessibility::Protected;
			break;
		}
		case MONO_FIELD_ATTR_PUBLIC: {
			accessibility = (uint8_t)Accessibility::Public;
			break;
		}
		}

		return accessibility;
	}*/


	//static // Gets the accessibility level of the given property
	//		uint8_t
	//		GetPropertyAccessbility(MonoProperty* property)
	//{
	//	uint8_t accessibility = (uint8_t)Accessibility::None;

	//	// Get a reference to the property's getter method
	//	MonoMethod* propertyGetter = mono_property_get_get_method(property);
	//	if (propertyGetter != nullptr)
	//	{
	//		// Extract the access flags from the getters flags
	//		uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

	//		switch (accessFlag)
	//		{
	//		case MONO_FIELD_ATTR_PRIVATE: {
	//			accessibility = (uint8_t)Accessibility::Private;
	//			break;
	//		}
	//		case MONO_FIELD_ATTR_FAM_AND_ASSEM: {
	//			accessibility |= (uint8_t)Accessibility::Protected;
	//			accessibility |= (uint8_t)Accessibility::Internal;
	//			break;
	//		}
	//		case MONO_FIELD_ATTR_ASSEMBLY: {
	//			accessibility = (uint8_t)Accessibility::Internal;
	//			break;
	//		}
	//		case MONO_FIELD_ATTR_FAMILY: {
	//			accessibility = (uint8_t)Accessibility::Protected;
	//			break;
	//		}
	//		case MONO_FIELD_ATTR_FAM_OR_ASSEM: {
	//			accessibility |= (uint8_t)Accessibility::Private;
	//			accessibility |= (uint8_t)Accessibility::Protected;
	//			break;
	//		}
	//		case MONO_FIELD_ATTR_PUBLIC: {
	//			accessibility = (uint8_t)Accessibility::Public;
	//			break;
	//		}
	//		}
	//	}

	//	// Get a reference to the property's setter method
	//	MonoMethod* propertySetter = mono_property_get_set_method(property);
	//	if (propertySetter != nullptr)
	//	{
	//		// Extract the access flags from the setters flags
	//		uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
	//		if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
	//			accessibility = (uint8_t)Accessibility::Private;
	//	}
	//	else
	//	{
	//		accessibility = (uint8_t)Accessibility::Private;
	//	}

	//	return accessibility;
	//}


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
		if (CheckMonoError(error))
			return "";
		std::string result(utf8);
		mono_free(utf8);
		return result;
	}


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

		MonoAssembly* ass = LoadCSharpAssembly(CMAKE_SOURCE_DIR "/ScriptingSandbox/bin/Debug/ScriptingSandbox.dll");
		PrintAssemblyTypes(ass);

		// Get a reference to the class we want to instantiate
		MonoClass* testingClass=GetClassInAssembly(ass, "ScriptingSanbox", "CSharpTesting");

		// Allocate an instance of our class
		MonoObject* classInstance = mono_object_new(s_data->app_domain, testingClass);

		if (classInstance == nullptr)
		{
			CORE_ASSERT(false, "Failed to create instance of class");
		}

		// Call the parameterless (default) constructor
		mono_runtime_object_init(classInstance);


		////////////////////// call 
		MonoClass* instanceClass = mono_object_get_class(classInstance);
		{
		
		std::string method_name = "PrintFloatVar";

		MonoMethod* method = mono_class_get_method_from_name(instanceClass, method_name.c_str(), 0);

		if (method == nullptr)
		{
			CORE_ASSERT(false, "Thire is no method with name = {0}", method_name);
			return;
		}

		// Call the C# method on the objectInstance instance, and get any potential exceptions
		MonoObject* exception = nullptr;//for another side exeption (from script  side)
		mono_runtime_invoke(method, classInstance, nullptr, &exception);
		}

		{
		std::string method_name_2 = "IncrementFloatVar";

		MonoMethod* method = mono_class_get_method_from_name(instanceClass, method_name_2.c_str(), 1);

		if (method == nullptr)
		{
			CORE_ASSERT(false, "Thire is no method with name = {0}", method_name_2);
			return;
		}

		// Call the C# method on the objectInstance instance, and get any potential exceptions
		MonoObject* exception = nullptr;//for another side exeption (from script  side)

		float value = 10;
		void* param = &value;
		mono_runtime_invoke(method, classInstance, &param, &exception);
		}

		{

		std::string method_name = "PrintFloatVar";

		MonoMethod* method = mono_class_get_method_from_name(instanceClass, method_name.c_str(), 0);

		if (method == nullptr)
		{
			CORE_ASSERT(false, "Thire is no method with name = {0}", method_name);
			return;
		}

		// Call the C# method on the objectInstance instance, and get any potential exceptions
		MonoObject* exception = nullptr;//for another side exeption (from script  side)
		mono_runtime_invoke(method, classInstance, nullptr, &exception);
		}



		//// Get a reference to the public field called "MyPublicFloatVar"
		//MonoClassField* floatField = mono_class_get_field_from_name(testingClass, "MyPublicFloatVar");
		//uint8_t floatFieldAccessibility = GetFieldAccessibility(floatField);

		//if (floatFieldAccessibility & (uint8_t)Accessibility::Public)
		//{
		//	float value;
		//mono_field_get_value(classInstance, floatField, &value);

		//	// Increment value by 10 and assign it back to the variable
		//	value += 10.0f;
		//mono_field_set_value(classInstance, floatField, &value);
		//}


		//// Get a reference to the private field called "m_Name"
		//MonoClassField* nameField = mono_class_get_field_from_name(testingClass, "m_Name");
		//uint8_t nameFieldAccessibility = GetFieldAccessibility(nameField);

		//if (nameFieldAccessibility & (uint8_t)Accessibility::Private)
		//{
		//// We shouldn't write to this field
		//}


		//// Get a reference to the public property called "Name"
		//MonoProperty* nameProperty = mono_class_get_property_from_name(testingClass, "Name");
		//uint8_t namePropertyAccessibility = GetPropertyAccessbility(nameProperty);

		//if (namePropertyAccessibility & (uint8_t)Accessibility::Public)
		//{
		//MonoString* nameValue = (MonoString*)mono_property_get_value(nameProperty, classInstance, nullptr, nullptr);
		//std::string nameStr = MonoStringToUTF8(nameValue);

		//// Modify and assign the value back to the property by invoking the setter method
		//nameStr += ", World!";
		//nameValue = mono_string_new(s_data->app_domain, nameStr.c_str());
		//mono_property_set_value(nameProperty, classInstance, (void**)&nameValue, nullptr);
		//}



		delete ass;
	}
	void ScriptEngine::ShutDown()
	{
		delete s_data->root_domain;
		delete s_data->app_domain;
		delete s_data;
	}
} // Engine