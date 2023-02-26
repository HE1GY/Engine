#pragma once
#include <filesystem>
#include <Engine/Renderer/Texture.h>

namespace Engine
{

	class ContentPanel
	{
	public:
		ContentPanel();

		void OnImGuiRender();

	private:

		std::filesystem::path m_current_directory;
		Ref<Texture> m_file_texture;
		Ref<Texture> m_folder_texture;

		float m_icon_size{ 100 };
		float m_padding{ 10 };
	};

} // Engine
