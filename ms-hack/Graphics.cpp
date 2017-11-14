#include "Graphics.h"



Graphics::Graphics()
{
	pD2DFactory = NULL;
	pRT = NULL;
}


Graphics::~Graphics()
{
	SafeRelease(&pD2DFactory);
	SafeRelease(&pRT);
}

bool Graphics::Init()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (hr != S_OK)
		return false;

	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))), &pRT);
	if (hr != S_OK)
		return false;

	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &pBlackBrush);
	if (hr != S_OK)
		return false;

	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.3f), &pRedBrush);
	if (hr != S_OK)
		return false;

	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);
	if (hr != S_OK)
		return false;

	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.2f), &pGreenBrush);
	if (hr != S_OK)
		return false;

	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 0.2f), &pGrayBrush);
	if (hr != S_OK)
		return false;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), reinterpret_cast<IUnknown **>(&pDWriteFactory));
	if (hr != S_OK)
		return false;

	hr = pDWriteFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14, L"", &pTextFormat);
	if (hr != S_OK)
		return false;

	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (!m_MsM.Initialize())
		return false;

	HWND m_hWndTarget = FindWindow(NULL, "MineSweeper");
	if (!m_hWndTarget) {
		return false;
	}

	return true;
}

void Graphics::UpdatePosition()
{
	HWND m_hWndTarget = FindWindow(NULL, "MineSweeper");
	if (!m_hWndTarget) {
		ExitProcess(0);
	}
	RECT wndRect;
	GetWindowRect(m_hWndTarget, &wndRect);

	m_WindowRect = { wndRect.left, wndRect.top, wndRect.right, wndRect.bottom };

	SetWindowPos(m_hWnd, NULL, wndRect.left, wndRect.top, (wndRect.right - wndRect.left), (wndRect.bottom - wndRect.top), SWP_SHOWWINDOW);
}

void Graphics::setHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void Graphics::Paint()
{
	pRT->BeginDraw();
	pRT->Clear();
	
	UpdatePosition();
	pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	
	D2D1_RECT_F drawRect = { m_WindowRect.left + 15 , m_WindowRect.top + 100, m_WindowRect.right -12 , m_WindowRect.bottom -12 };

	m_MsM.Update();
	//m_MsM.Cheat();

	for (int i = 0; i < m_MsM.GetFieldWidth(); i++) {
		for (int j = 0; j < m_MsM.GetFieldHeight(); j++) {
			D2D1_RECT_F rect = convertRect(drawRect.left + SQUARESIZE * i, drawRect.top + j * SQUARESIZE, drawRect.left + (i + 1) * SQUARESIZE, drawRect.top + (j + 1) * SQUARESIZE, m_WindowRect); 
			if (m_MsM.IsMine(i, j)) {
				pRT->FillRectangle(&rect, pRedBrush);
			}
			else if (m_MsM.IsEmpty(i,j)) {
				pRT->FillRectangle(&rect, pGreenBrush);
			}
			else {
				pRT->FillRectangle(&rect, pGrayBrush);
			}
		}
	}

	HRESULT hr = pRT->EndDraw();
}

HWND Graphics::GetTargetHwnd()
{
	return m_hWndTarget;
}

D2D1_POINT_2U Graphics::convertCoordinates(D2D1_POINT_2U point, RECT wndRect)
{
	D2D1_POINT_2U result;
	result.x = static_cast<int>(((float)point.x - (float)wndRect.left) / ((float)wndRect.right - (float)wndRect.left)*GetSystemMetrics(SM_CXSCREEN));
	result.y = static_cast<int>(((float)point.y - (float)wndRect.top) / ((float)wndRect.bottom - (float)wndRect.top)*GetSystemMetrics(SM_CYSCREEN));
	return result;
}

D2D1_RECT_F Graphics::convertRect(int left, int top, int right, int bottom, RECT wndRect)
{
	D2D1_RECT_F result;
	result.left = (int)((float)left - (float)wndRect.left) / ((float)wndRect.right - (float)wndRect.left)*GetSystemMetrics(SM_CXSCREEN);
	result.top = (int)((float)top - (float)wndRect.top) / ((float)wndRect.bottom - (float)wndRect.top)*GetSystemMetrics(SM_CYSCREEN);
	result.right = (int)((float)right - (float)wndRect.left) / ((float)wndRect.right - (float)wndRect.left)*GetSystemMetrics(SM_CXSCREEN);
	result.bottom = (int)((float)bottom - (float)wndRect.top) / ((float)wndRect.bottom - (float)wndRect.top)*GetSystemMetrics(SM_CYSCREEN);
	return result;
}

int Graphics::convertxInt(int xval, RECT wndRect)
{
	return static_cast<int>(((float)xval - (float)wndRect.left) / ((float)wndRect.right - (float)wndRect.left)*GetSystemMetrics(SM_CXSCREEN));
}

int Graphics::convertyInt(int yval, RECT wndRect)
{
	return static_cast<int>(((float)yval - (float)wndRect.top) / ((float)wndRect.bottom - (float)wndRect.top)*GetSystemMetrics(SM_CYSCREEN));
}