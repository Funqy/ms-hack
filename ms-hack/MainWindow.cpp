#include "MainWindow.h"




MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}

HRESULT MainWindow::Initialize()
{

	WNDCLASSEX wndClass;

	m_hInstance = GetModuleHandle(NULL);

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = "MainMenu";
	wndClass.lpszClassName = "SnekClass";
	wndClass.hIconSm = (HICON)LoadImage(m_hInstance, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wndClass)) {
		LPTSTR buffer = new TCHAR[256];
		wsprintf(buffer, "RegisterClassEx failed!\n\nError Code:\t%d", GetLastError());
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION);
		delete buffer;
		return false;
	}

	HWND hWnd = FindWindow(NULL, "MineSweeper");
	if (!hWnd) {
		LPTSTR buffer = new TCHAR[256];
		wsprintf(buffer, "FindWindow failed!\n\nError Code:\t%d", GetLastError());
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION);
		delete buffer;
		return false;
	}

	RECT wndRect;
	GetWindowRect(hWnd, &wndRect);

	m_hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, "SnekClass", "Overlay", WS_POPUP, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom, NULL, NULL, m_hInstance, this);

	if (!m_hWnd) {
		LPTSTR buffer = new TCHAR[256];
		wsprintf(buffer, "CreateWindowEx failed!\n\nError Code:\t%d", GetLastError());
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION);
		delete buffer;
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	const MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(m_hWnd, &margin); //Perhaps with DC Rendertarget not necessary
	GraphicInterface.setHwnd(m_hWnd);
	if (!GraphicInterface.Init())
		return false;

	return true;
}

void MainWindow::RunMessageLoop()
{
	MSG msg;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			GraphicInterface.Paint();
		}
	}
}


LRESULT MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool wasHandled = false;

	LRESULT result = 0;
	if (uMsg == WM_CREATE) {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		::SetWindowLongPtrA(hWnd, GWLP_USERDATA, (ULONG)pcs->lpCreateParams);
		::SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

		result = 1;

		wasHandled = true;
	}
	else {
		MainWindow* mWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (uMsg == WM_DESTROY) {
			PostQuitMessage(0);
			wasHandled = true;
		}
	}

	if (!wasHandled) {
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return result;
}
