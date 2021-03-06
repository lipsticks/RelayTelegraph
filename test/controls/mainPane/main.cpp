#include "../../../modules/lib/win/win.h"
#include "../../../modules/app/win/TourneyListView/TourneyListView.h"
#include "../../../controls/mainPane/mainPane.h"
#include "resource.h"

LRESULT CALLBACK MainWindowProc(HWND h, UINT m, WPARAM w, LPARAM l) {
	enum {ID_NONE=1000,ID_RT,ID_TL};
	static lib::win::WindowHandle rt;
	static app::win::TourneyListView tl;
	
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
			tl.Create(h, ID_TL);
			
			lib::win::debug::out("ok");
			
			tl.AddTourney("2013-02-20", "06:00", "Karkonosze Open Group A", "5");
			tl.AddTourney("2013-02-21", "00:00", "Cen.Serbia Champ's Men", "5");
			return 0;
		}
		case WM_SIZE: {
			const int cx=LOWORD(l), cy=HIWORD(l);
			const int tlHeight = cy-200;
			SetWindowSize(tl, cx, tlHeight);
			MoveWindow(rt, 0, tlHeight, cx, cy, FALSE);
			
			tl.FitHeaderToClient();
			return 0;
		}
		case WM_ERASEBKGND: {
			return 0;
		}
		case WM_NOTIFY: {
			NMHDR &nmhdr = *(LPNMHDR)l;
			if(ID_TL == w) {
				switch(nmhdr.code) {
					case LVN_ITEMCHANGED: {
						NMLISTVIEW &nmlv = *(LPNMLISTVIEW)l;
						if(nmlv.uNewState & (LVIS_SELECTED))
							lib::win::debug::out("LVN_ITEMCHANGED (selected): %d", nmlv.iItem);
						else if(nmlv.uNewState & LVIS_FOCUSED)
							lib::win::debug::out("LVN_ITEMCHANGED (focused): %d", nmlv.iItem);
						break;
					}
					case LVN_ITEMACTIVATE: {
						NMITEMACTIVATE &nmia = *(LPNMITEMACTIVATE)l;
						lib::win::debug::out("LVN_ITEMACTIVATE: %d", nmia.iItem);
						break;
					}
					case LVN_COLUMNCLICK: {
						NMLISTVIEW &nmlv = *(LPNMLISTVIEW)l;
						lib::win::debug::out("LVN_COLUMNCLICK: %d", nmlv.iSubItem);
						break;
					}
					case NM_RCLICK : {
						NMLISTVIEW &nmlv = *(LPNMLISTVIEW)l;
						lib::win::debug::out("NM_RCLICK : %d %d", nmlv.iItem, nmlv.iSubItem);
						break;
					}
				}
			}
			else if(nmhdr.hwndFrom == (HWND)SendMessage(tl, LVM_GETHEADER, 0, 0)) {
				switch(nmhdr.code) {
					case HDN_ENDTRACK:
						lib::win::debug::out("HDN_ENDTRACK");
						break;
				}
			}
		}
	}
	return DefWindowProc(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR args, int show)
{
	lib::win::InitSpecificCommonControls(ICC_LISTVIEW_CLASSES);
	
	HICON icon = LoadIcon(instance, MAKEINTRESOURCE(ID_ICO_APPLICATION));
	lib::win::WindowClass wcmw("MAIN_WINDOW_CLASS", MainWindowProc, NULL, icon);
	lib::win::WindowClass wcrt("RELAY_TELEGRAPH_CLASS", MainPaneProc);
	
	lib::win::WindowHandle hwnd;
	hwnd.Create(wcmw, "MainPaneClass Tester", WS_OVERLAPPEDWINDOW);
	SetWindowSize(hwnd, 480, 400);
	ShowWindow(hwnd, SW_SHOW);
	
	lib::win::EnterMessageLoop();
	
	return 0;
}
