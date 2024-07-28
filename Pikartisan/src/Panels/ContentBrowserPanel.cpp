#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Pika {

	// TODO : Use project path
	constexpr const char* s_BaseDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory{ s_BaseDirectory }, m_CurrentDirectory{ m_BaseDirectory }
	{
		m_FileIcon = Texture2D::Create("resources/icons/ContentBrowserPanel/FileIcon.png");
		m_DirectoryIcon = Texture2D::Create("resources/icons/ContentBrowserPanel/DirectoryIcon.png");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		ImGui::Begin("Content Browser");
		uintptr_t DirectoryIcon = static_cast<uintptr_t>(m_DirectoryIcon->getRendererID());
		uintptr_t FileIcon = static_cast<uintptr_t>(m_FileIcon->getRendererID());

		if (m_CurrentDirectory != m_BaseDirectory) {
			if (ImGui::Button("<"))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}

		// 设置Icon ColumnCount
		static float IconSize = 128.0f;
		static float IconPadding = 8.0f;
		float CellSize = IconSize + IconPadding;
		auto PanelWidth = ImGui::GetContentRegionAvail().x;
		int ColumnCount = static_cast<int>(PanelWidth / CellSize);
		if (ColumnCount < 1)
			ColumnCount = 1;

		ImGui::Columns(ColumnCount, 0, false); // 设置false，不需要border
		for (auto& DirectoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const auto& Path = DirectoryEntry.path(); // 当前目录文件和子目录路径
			std::string Filename = Path.filename().string(); // 文件名

			ImGui::BeginGroup();
			uintptr_t Icon = DirectoryEntry.is_directory() ? DirectoryIcon : FileIcon;
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(Icon), { IconSize, IconSize });

			if (ImGui::BeginDragDropSource()) {
				ImGui::EndDragDropSource();
			}

			// 居中对齐显示
			float ColumnWidth = ImGui::GetColumnWidth();
			ImVec2 TextSize = ImGui::CalcTextSize(Filename.c_str());
			float TextOffsetX = (ColumnWidth - TextSize.x) / 2.0f;
			TextOffsetX = TextOffsetX > 2.0f ? TextOffsetX : 2.0f; // 不允许小于2.0f
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TextOffsetX);
			ImGui::TextWrapped(Filename.c_str());
			ImGui::EndGroup();
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if(DirectoryEntry.is_directory())
					m_CurrentDirectory /= Filename;
			}

			ImGui::NextColumn();
		}
		ImGui::Columns();
		ImGui::End();
	}

}