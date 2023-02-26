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
		m_file_texture = Texture2D::Create(CMAKE_SOURCE_DIR"/EngineEditor/resources/icon/FileIcon.png");
		m_folder_texture = Texture2D::Create(CMAKE_SOURCE_DIR"/EngineEditor/resources/icon/DirectoryIcon.png");
	}

	void ContentPanel::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

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

			if (ImGui::GetCursorPosX() > ImGui::GetContentRegionMax().x)
			{
				ImGui::NewLine();
			}

			if (entry.is_directory())
			{
				ImGui::BeginGroup();
				if (ImGui::ImageButton(relative.string().c_str(), (ImTextureID)m_folder_texture->GetRendererId(),
						{ m_icon_size, m_icon_size }, { 1, 1 }, { 0, 0 }))
				{
					m_current_directory = entry.path();
				}
				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + m_icon_size);
				float text_pos_x = ImGui::GetCursorPosX() + (m_icon_size / 2.0f)
						- ImGui::CalcTextSize(relative.string().c_str(), NULL, false, m_icon_size).x / 2.0f;

				ImGui::SetCursorPosX(text_pos_x);
				ImGui::Text(relative.string().c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();

				ImGui::SameLine(0, m_padding);
			}
			else
			{
				ImGui::BeginGroup();
				if (ImGui::ImageButton(relative.string().c_str(), (ImTextureID)m_file_texture->GetRendererId(),
						{ m_icon_size, m_icon_size }, { 1, 1 }, { 0, 0 }))
				{
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					if (entry.path().extension() == ".engine")
					{
						size_t path_size = strlen(entry.path().string().c_str()) * sizeof(std::basic_string<char>);
						ImGui::SetDragDropPayload("DND_SCENE", entry.path().string().c_str(), path_size);
					}
					ImGui::EndDragDropSource();
				}

				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + m_icon_size);
				float text_pos_x = ImGui::GetCursorPosX() + (m_icon_size / 2.0f)
						- ImGui::CalcTextSize(relative.string().c_str(), NULL, false, m_icon_size).x / 2.0f;

				ImGui::SetCursorPosX(text_pos_x);
				ImGui::Text(relative.string().c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();

				ImGui::SameLine(0, m_padding);
			}
		}

		ImGui::NewLine();
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 60);
		ImGui::SliderFloat("Icon size", &m_icon_size, 50.0f, 100.0f, "%.1f");
		ImGui::SliderFloat("Padding", &m_padding, 10.0f, 40.0f, "%.1f");

		ImGui::End();
	}

} // Engine