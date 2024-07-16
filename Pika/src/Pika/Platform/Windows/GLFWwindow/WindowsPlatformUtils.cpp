#include "pkpch.h"
#include "Pika/Utils/PlatformUtils.h"
#include "Pika/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32 // GLFW 暴露底层win32接口
#include <GLFW/glfw3native.h>

namespace Pika {

	std::string FileDialogs::OpenFile(const char* vFilter) {
		OPENFILENAMEA ofn;   // 定义一个 OPENFILENAMEA 结构体，用于配置文件对话框的选项
		CHAR szFile[260] = { 0 }; // 定义一个字符数组 szFile，用于存储选定文件的路径
		CHAR currentDir[256] = { 0 };  // 定义一个字符数组 currentDir，用于存储当前目录
		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 将 OPENFILENAME 结构体的内存清零
		ofn.lStructSize = sizeof(OPENFILENAME);  // 设置 OPENFILENAME 结构体的大小
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().getWindow().getNativeWindow());  // 设置文件对话框的所有者窗口句柄
		ofn.lpstrFile = szFile;  // 设置用于存储文件路径的缓冲区
		ofn.nMaxFile = sizeof(szFile); // 设置缓冲区大小
		if (GetCurrentDirectoryA(256, currentDir)) // 获取当前目录，并将其设置为文件对话框的初始目录
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = vFilter; // 设置文件过滤器
		ofn.nFilterIndex = 1; // 设置过滤器索引
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // 设置文件对话框的标志，要求路径和文件必须存在，不允许更改目录

		if (GetOpenFileNameA(&ofn) == TRUE)  // 显示文件对话框，如果用户选择了一个文件，返回文件路径
			return ofn.lpstrFile;

		return std::string();  // 如果用户取消了对话框，返回空字符串
	}

	std::string FileDialogs::SaveFile(const char* vFilter) {
		OPENFILENAMEA ofn; // 定义一个 OPENFILENAMEA 结构体变量，用于传递和接收对话框的信息。
		CHAR szFile[260] = { 0 }; // 定义一个字符数组，用于存储用户选择的文件路径。
		CHAR currentDir[256] = { 0 }; // 定义一个字符数组，用于存储当前目录路径。
		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 将 ofn 结构体变量的内存初始化为零。
		ofn.lStructSize = sizeof(OPENFILENAME); // 设置结构体的大小。
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().getWindow().getNativeWindow()); // 设置对话框的所有者窗口句柄。
		ofn.lpstrFile = szFile; // 设置用于存储文件路径的字符数组。
		ofn.nMaxFile = sizeof(szFile); // 设置文件路径字符数组的大小。
		if (GetCurrentDirectoryA(256, currentDir)) // 获取当前目录路径并存储在 currentDir 中。
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = vFilter; // 设置文件类型过滤器。
		ofn.nFilterIndex = 1; // 设置过滤器索引。
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR; // 设置对话框标志，要求路径必须存在、提示覆盖文件、不改变当前目录。

		ofn.lpstrDefExt = strchr(vFilter, '\0') + 1; // 设置默认扩展名，从过滤器字符串中提取。

		if (GetSaveFileNameA(&ofn) == TRUE) // 显示保存文件对话框，如果用户选择了文件，则返回文件路径。
			return ofn.lpstrFile;

		return std::string(); // 如果用户取消了对话框，返回空字符串。
	}

}