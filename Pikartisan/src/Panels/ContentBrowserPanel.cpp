#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Pika {

	// TODO : Delete! Use project path 
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
		static float IconPadding = 8.0f; // 左右分别IconPadding / 2
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
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + IconPadding / 2.0f);
			ImGui::PushID(Filename.c_str());
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(Icon), { IconSize, IconSize }, { 0,1 }, { 1,0 });
			// 可直接拖拽UI
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* ItemPath = Path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", ItemPath, (wcslen(ItemPath) + 1) * sizeof(wchar_t)); // +1包含终止符'\0'
				ImGui::EndDragDropSource();
			}

			// 居中对齐显示
			float ColumnWidth = ImGui::GetColumnWidth();
			ImVec2 TextSize = ImGui::CalcTextSize(Filename.c_str());
			float TextOffsetX = (ColumnWidth - TextSize.x) / 2.0f;
			TextOffsetX = TextOffsetX > 2.0f ? TextOffsetX : 2.0f; // 不允许小于2.0f
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TextOffsetX);
			ImVec2 TextStartPos = ImGui::GetCursorScreenPos(); // Text左上
			TextStartPos.x -= 1, TextStartPos.y += TextSize.y; // 调整位置
			ImGui::TextWrapped(Filename.c_str());
			// 文本鼠标悬停下划线
			if (ImGui::IsItemHovered()) {
				ImVec2 TextEndPos{ TextStartPos.x + TextSize.x, TextStartPos.y };
				ImDrawList* DrawList = ImGui::GetWindowDrawList(); // ImGui的绘制API
				DrawList->AddLine(TextStartPos, TextEndPos, IM_COL32(200, 200, 200, 255));
			}
			ImGui::PopID();
			ImGui::EndGroup();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (DirectoryEntry.is_directory())
					m_CurrentDirectory /= Filename;
			}

			ImGui::NextColumn();
		}
		ImGui::Columns();
		ImGui::End();
	}

}