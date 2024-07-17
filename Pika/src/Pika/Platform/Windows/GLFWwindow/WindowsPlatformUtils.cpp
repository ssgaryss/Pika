#include "pkpch.h"
#include "Pika/Utils/PlatformUtils.h"
#include "Pika/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32 // GLFW ��¶�ײ�win32�ӿ�
#include <GLFW/glfw3native.h>

namespace Pika {

	std::string FileDialogs::OpenFile(const char* vFilter) {
		OPENFILENAMEA ofn;   // ����һ�� OPENFILENAMEA �ṹ�壬���������ļ��Ի����ѡ��
		CHAR szFile[260] = { 0 }; // ����һ���ַ����� szFile�����ڴ洢ѡ���ļ���·��
		CHAR currentDir[256] = { 0 };  // ����һ���ַ����� currentDir�����ڴ洢��ǰĿ¼
		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // �� OPENFILENAME �ṹ����ڴ�����
		ofn.lStructSize = sizeof(OPENFILENAME);  // ���� OPENFILENAME �ṹ��Ĵ�С
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().getWindow().getNativeWindow());  // �����ļ��Ի���������ߴ��ھ��
		ofn.lpstrFile = szFile;  // �������ڴ洢�ļ�·���Ļ�����
		ofn.nMaxFile = sizeof(szFile); // ���û�������С
		if (GetCurrentDirectoryA(256, currentDir)) // ��ȡ��ǰĿ¼������������Ϊ�ļ��Ի���ĳ�ʼĿ¼
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = vFilter; // �����ļ�������
		ofn.nFilterIndex = 1; // ���ù���������
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // �����ļ��Ի���ı�־��Ҫ��·�����ļ�������ڣ����������Ŀ¼

		if (GetOpenFileNameA(&ofn) == TRUE)  // ��ʾ�ļ��Ի�������û�ѡ����һ���ļ��������ļ�·��
			return ofn.lpstrFile;

		return std::string();  // ����û�ȡ���˶Ի��򣬷��ؿ��ַ���
	}

	std::string FileDialogs::SaveFile(const char* vFilter) {
		OPENFILENAMEA ofn; // ����һ�� OPENFILENAMEA �ṹ����������ڴ��ݺͽ��նԻ������Ϣ��
		CHAR szFile[260] = { 0 }; // ����һ���ַ����飬���ڴ洢�û�ѡ����ļ�·����
		CHAR currentDir[256] = { 0 }; // ����һ���ַ����飬���ڴ洢��ǰĿ¼·����
		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // �� ofn �ṹ��������ڴ��ʼ��Ϊ�㡣
		ofn.lStructSize = sizeof(OPENFILENAME); // ���ýṹ��Ĵ�С��
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().getWindow().getNativeWindow()); // ���öԻ���������ߴ��ھ����
		ofn.lpstrFile = szFile; // �������ڴ洢�ļ�·�����ַ����顣
		ofn.nMaxFile = sizeof(szFile); // �����ļ�·���ַ�����Ĵ�С��
		if (GetCurrentDirectoryA(256, currentDir)) // ��ȡ��ǰĿ¼·�����洢�� currentDir �С�
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = vFilter; // �����ļ����͹�������
		ofn.nFilterIndex = 1; // ���ù�����������
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR; // ���öԻ����־��Ҫ��·��������ڡ���ʾ�����ļ������ı䵱ǰĿ¼��

		ofn.lpstrDefExt = strchr(vFilter, '\0') + 1; // ����Ĭ����չ�����ӹ������ַ�������ȡ��

		if (GetSaveFileNameA(&ofn) == TRUE) // ��ʾ�����ļ��Ի�������û�ѡ�����ļ����򷵻��ļ�·����
			return ofn.lpstrFile;

		return std::string(); // ����û�ȡ���˶Ի��򣬷��ؿ��ַ�����
	}

}