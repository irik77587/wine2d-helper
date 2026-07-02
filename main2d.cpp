#include "main2d.h"

#define FORM "FormClass"

void DrawTextDWrite(RECT rc);

// Direct2D Variables
ID2D1Factory* pD2D1Factory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pBlackBrush = NULL;

void Init_D2D(HWND hwnd) {
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2D1Factory
	);
	RECT rc;
	GetClientRect(hwnd, &rc);
	hr = pD2D1Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			)
		),
		&pRT
	);
	if(SUCCEEDED(hr)) {
		pRT->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush
		);
	}
}

void render_frame(HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);
	pRT->Resize(
		D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		)
	);

	pRT->BeginDraw();
	pRT->SetTransform(
		D2D1::IdentityMatrix()
	);
	pRT->Clear(
		D2D1::ColorF(
			D2D1::ColorF::White
		)
	);
	pRT->DrawRectangle(
		D2D1::RectF(
			rc.left + 100.0f,
			rc.top + 100.0f,
			rc.right - 100.0f,
			rc.bottom - 100.0f),
			pBlackBrush, 1.0f
	);
	DrawTextDWrite(rc);
	pRT->EndDraw();
}

template<typename Interface>
inline void SafeRelease(Interface **pInterface) {
	if(*pInterface != NULL) {
		(*pInterface)->Release();
		(*pInterface) = NULL;
	}
}

void cleanD2D() {
	SafeRelease(&pRT);
	SafeRelease(&pBlackBrush);
	SafeRelease(&pD2D1Factory);
}

// DirectWrite Variables
IDWriteFactory* pDWriteFactory;
IDWriteTextFormat* pTextFormat;

void Init_dwrite() {
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
	);

	if(SUCCEEDED(hr)) {
		hr = pDWriteFactory->CreateTextFormat(
			L"Gabriola",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			70.0f, L"en-us", &pTextFormat
		);
	}

	if(SUCCEEDED(hr)) {
		hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if(SUCCEEDED(hr)) {
		hr = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

}

void DrawTextDWrite(RECT rc) {
	D2D1_RECT_F layoutRect = D2D1::RectF(
		rc.left + 200.0f,
		rc.top + 200.0f,
		rc.right - 200.0f,
		rc.bottom - 200.0f
	);

	const wchar_t* wszText = L"Hello World using DirectWrite!";
	UINT32 cTextLength = (UINT32) wcslen(wszText);

	pRT->DrawText(wszText, cTextLength, pTextFormat, layoutRect, pBlackBrush);
}

void clean_dwrite() {
	SafeRelease(&pTextFormat);
	SafeRelease(&pDWriteFactory);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  {
	switch(message) {
		case WM_CREATE: {
			Init_D2D(hWnd);
			Init_dwrite();
			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			// 2d rendering is done outside window procedure
			// only gdi is rendered here
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc (hWnd, message, wParam, lParam);
	}
	return 0;
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;
	WNDCLASSEX wc;
	MSG msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WindowProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = FORM;
	wc.hIcon		 = NULL;
	wc.hIconSm		 = NULL;

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0, FORM, "Direct2D Example", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// enter the main loop
	while(msg.message != WM_QUIT) {
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		render_frame(hwnd);
	}
	clean_dwrite();
	cleanD2D();

	return msg.wParam;
}
