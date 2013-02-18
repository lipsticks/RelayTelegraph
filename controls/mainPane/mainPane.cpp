#include "mainPane.h"
#include "../../modules/lib/win/win.h"
#include <string.h>

LRESULT CALLBACK MainPaneProc(HWND h, UINT m, WPARAM w, LPARAM l) {
	static RECT r = {10, 10, 100, 100};
	static bool on = false;
	
	switch(m) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(h, &ps);
			SaveDC(hdc);
			
			COLORREF c = on ? RGB(220,200,180) : RGB(255,255,255);
			HBRUSH hb = CreateSolidBrush(c);
			SelectObject(hdc, hb);
			
			Rectangle(hdc, r.left, r.top, r.right, r.bottom);
			const char * message = "<- click the box to change the color";
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, r.left + r.right + 10, r.top + (r.bottom-r.top)>>1, message, strlen(message));
			
			DeleteObject(hb);
			RestoreDC(hdc, -1);
			EndPaint(h, &ps);
			return 0;
		}
		
		case WM_LBUTTONUP: {
			POINT p = {LOWORD(l), HIWORD(l)};
			if(PtInRect(&r, p)) {
				on = !on;
				InvalidateRect(h, NULL, FALSE);
				UpdateWindow(h);
			}
			return 0;
		}
	}
	return DefWindowProc(h, m, w, l);
}
