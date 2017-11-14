#include <Windows.h>
#include "Graphics.h"
#include <iostream>
#include <Dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

#pragma once
class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	HRESULT Initialize();

	void RunMessageLoop();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Graphics GraphicInterface;
	HWND m_hWnd;
	HINSTANCE m_hInstance;

};
