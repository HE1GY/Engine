#include "pch.h"
#include "ContentPanel.h"

#include <imgui.h>

#include "config.h"

namespace Engine
{
	static const std::filesystem::path s_root = CMAKE_SOURCE_DIR"/EngineEditor/assets";

	ContentPanel::ContentPanel()
			:m_current_directory(s_root)
	{
	}

	void ContentPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_current_directory != s_root)
		{
			if (ImGui::Button("<--"))
			{
				m_current_directory = m_current_directory.parent_path();
			}
		}

		for (const auto& entry : std::filesystem::directory_iterator(m_current_directory))
		{
			std::filesystem::path relative = std::filesystem::relative(entry, m_current_directory);
			if (entry.is_directory())
			{
				if (ImGui::Button(relative.string().c_str()))
				{
					m_current_directory = entry.path();
				}
			}
			else
			{
				if (ImGui::Button(relative.string().c_str()))
				{

				}
			}
		}

		ImGui::End();
	}

} // Engine