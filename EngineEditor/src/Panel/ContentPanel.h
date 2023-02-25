#pragma once
#include <filesystem>

namespace Engine
{

	class ContentPanel
	{
	public:
		ContentPanel();

		void OnImGuiRender();

	private:

		std::filesystem::path m_current_directory;
	};

} // Engine
