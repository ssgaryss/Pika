#pragma once
#include <Pika.h>
#include <filesystem>

namespace Pika {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		inline bool* getIsShowContentBrowser() { return &m_IsShowContentBrowser; }

		void onImGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		// Icons
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_DirectoryIcon;
		bool m_IsShowContentBrowser = true;
	};

}

