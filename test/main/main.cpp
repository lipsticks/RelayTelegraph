#include "../../modules/lib/win/win.h"
#include "resource.h"

//TODO: Add a server output simulator. That is, capture a couple of minutes of
//  the BabasChess console and play it back here in a loop. Additionally, add
//  a little intelligence by implementing pre-recorded responses for the couple
//  of server commands sent by the plugin.

HMODULE plugin = NULL;
HWND pane = NULL;

LRESULT CALLBACK MainWindowProc(HWND h, UINT m, WPARAM w, LPARAM l) {
	enum {ID_NONE=1000,ID_RT};
	static lib::win::WindowHandle rt;
	
	switch(m) {
		case WM_CLOSE: DestroyWindow(h); return 0;
		case WM_DESTROY: {
			//if(plugin) FreeLibrary(plugin);
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN: {
			switch(w) {
				case VK_ESCAPE: SendMessage(h, WM_CLOSE, 0, 0); break;
			}
			return 0;
		}
		case WM_CREATE: {
			plugin = LoadLibrary("RelayTelegraph.BCPlugin");
			if(plugin) {
				typedef HWND (__cdecl *TBCP_CreateInfoTabWindow)(const GUID *pGUID,HWND hWndParent);
				TBCP_CreateInfoTabWindow f = (TBCP_CreateInfoTabWindow)GetProcAddress(plugin, "BCP_CreateInfoTabWindow");
				//lib::win::debug::out("%08X", f);
				pane = f(NULL, h);
			}
			return 0;
		}
		case WM_SIZE: {
			const int cx=LOWORD(l), cy=HIWORD(l);
			MoveWindow(pane, 0, 0, cx, cy, FALSE);
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
	
	HICON icon = LoadIcon(instance, MAKEINTRESOURCE(ID_ICO_APPLICATION));
	lib::win::WindowClass wcmw("MAIN_WINDOW_CLASS", MainWindowProc, NULL, icon);
	
	lib::win::WindowHandle hwnd;
	hwnd.Create(wcmw, "MainPaneClass Tester", WS_OVERLAPPEDWINDOW);
	SetWindowSize(hwnd, 480, 400);
	ShowWindow(hwnd, SW_SHOW);
	
	lib::win::EnterMessageLoop();
	
	return 0;
}
