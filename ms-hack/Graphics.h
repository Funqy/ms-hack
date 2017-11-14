#pragma once
#include <dwrite.h>
#include <wchar.h>
#include <chrono>
#include <vector>
#include <d2d1.h>
#include <algorithm>
#include "MinesweeperMemory.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#define SQUARESIZE 0x10

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init();

	void UpdatePosition();
	void setHwnd(HWND);
	void Paint();

	HWND GetTargetHwnd();

	D2D1_POINT_2U convertCoordinates(D2D1_POINT_2U,RECT);
	D2D1_RECT_F convertRect(int, int, int, int, RECT wndRect);
	int convertxInt(int, RECT);
	int convertyInt(int, RECT);

private:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1DCRenderTarget* dcRT;
	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1SolidColorBrush* pWhiteBrush;
	ID2D1SolidColorBrush* pGreenBrush;
	ID2D1SolidColorBrush* pRedBrush;
	ID2D1SolidColorBrush* pGrayBrush;

	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;

	MinesweeperMemory m_MsM;

	HWND m_hWnd;
	HWND m_hWndTarget;

	RECT m_WindowRect;
	D2D1_SIZE_U m_targetSize;
};

template <class T> void SafeRelease(T *ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
