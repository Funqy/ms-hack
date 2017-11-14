#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainWindow mWindow;

	if (mWindow.Initialize()) {
		mWindow.RunMessageLoop();
	}
	else {
		MessageBox(NULL, "Window not created!", "ERROR", MB_OK);
	}
}