#include "../../../modules/lib/win/win.h"
#include "../../../controls/mainPane/mainPane.h"
#include "resource.h"

LRESULT CALLBACK MainWindowProc(HWND h, UINT m, WPARAM w, LPARAM l) {
	static lib::win::WindowHandle rt;
	enum {ID_RT=1001};
	
	switch(m) {
		case WM_CLOSE: DestroyWindow(h); return 0;
		case WM_DESTROY: PostQuitMessage(0); return 0;
		case WM_KEYDOWN: {
			switch(w) {
				case VK_ESCAPE: SendMessage(h, WM_CLOSE, 0, 0); break;
			}
			return 0;
		}
		case WM_CREATE: {
			rt.Create("RELAY_TELEGRAPH_CLASS", h, ID_RT);
			return 0;
		}
		case WM_SIZE: {
			const int cx=LOWORD(l), cy=HIWORD(l);
			SetWindowSize(rt, cx, cy);
			return 0;
		}
		case WM_ERASEBKGND: {
			return 0;
		}
	}
	return DefWindowProc(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR args, int show)
{
	lib::win::InitSpecificCommonControls(ICC_LISTVIEW_CLASSES);
	
	lib::win::WindowClass wcrt("RELAY_TELEGRAPH_CLASS", MainPaneProc);
	
	HICON icon = LoadIcon(instance, MAKEINTRESOURCE(ID_ICO_APPLICATION));
	lib::win::WindowClass wcmw("MAIN_WINDOW_CLASS", MainWindowProc, NULL, icon);
	lib::win::WindowHandle hwnd;
	
	hwnd.Create(wcmw, "MainPaneClass Tester", WS_OVERLAPPEDWINDOW);
	SetWindowSize(hwnd, 350, 400);
	ShowWindow(hwnd, SW_SHOW);
	
	lib::win::EnterMessageLoop();
	
	return 0;
}
