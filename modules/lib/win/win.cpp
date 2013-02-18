#include "win.h"


void lib::win::InitSpecificCommonControls(DWORD flags) {
	INITCOMMONCONTROLSEX ccx;
	memset(&ccx, 0, sizeof(ccx));
	ccx.dwSize = sizeof(ccx);
	ccx.dwICC = flags;
	InitCommonControlsEx(&ccx);
}


BOOL lib::win::CreateSimpleWindowClass(WNDCLASSEX &wc, LPCSTR name, WNDPROC callback, HBRUSH brush, HICON icon, HINSTANCE instance) {
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	wc.lpfnWndProc = callback ? callback : DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance ? instance : GetModuleHandle(0);
	wc.hIcon = wc.hIconSm = icon ? icon : LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = brush ? brush : (HBRUSH) COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name;
	
	if(!RegisterClassEx(&wc)) {
		char buffer[1024];
		snprintf(buffer, sizeof(buffer), "Error while registering window class '%s': '%x'.",
			wc.lpszClassName, GetLastError());
		OutputDebugString(buffer);
		
		return FALSE;
	}
	
	return TRUE;
}


BOOL lib::win::UnregisterWindowClass(const WNDCLASSEX &wc) {
	if(!UnregisterClass(wc.lpszClassName, wc.hInstance)) {
		OutputDebugString("Error while un-registering window class.");
		return FALSE;
	}
	
	return TRUE;
}


HWND lib::win::InstantiateWindowClass(const WNDCLASSEX &wc, LPCSTR title, DWORD style, DWORD stylex, int x, int y, int w, int h) {
	HWND hwnd = CreateWindowEx(stylex, wc.lpszClassName, title, style,
		x, y, w, h, NULL, 0, wc.hInstance, NULL);
	
	if(!hwnd)
		OutputDebugString("Error while creating window.");
	
	return hwnd;
}


int lib::win::EnterMessageLoop() {
	MSG msg;
	
	while(true) {
		DWORD result = GetMessage(&msg, 0, 0, 0);
		
		if(-1 == result) {
			char buffer[1024];
			snprintf(buffer, sizeof(buffer), "Error in message loop: '%x'!", GetLastError());
			OutputDebugString(buffer);
		}
		
		if(0 == result)
			break;
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}


void lib::win::SetWindowSize(HWND h, int cx, int cy) {
	SetWindowPos(h, 0, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}



lib::win::WindowClass::WindowClass(LPCSTR name, WNDPROC callback, HBRUSH brush, HICON icon, HINSTANCE instance) {
	handle = lib::win::CreateSimpleWindowClass(wc, name, callback, brush, icon, instance);
	
	if(!handle)
		throw WinError(GetLastError(), "WindowClass: unable to register windowclass");
}

lib::win::WindowClass::~WindowClass() {
	if(handle) lib::win::UnregisterWindowClass(wc);
}

BOOL lib::win::WindowClass::IsRegistered() const {
	return !!handle;
}

HWND lib::win::WindowClass::Instantiate(LPCSTR title, DWORD style, DWORD stylex, int x, int y, int w, int h) const {
	HWND hwnd = lib::win::InstantiateWindowClass(wc, title, style, stylex, x, y, w, h);
	
	if(!hwnd)
		throw WinError(GetLastError(), "WindowClass: unable to instantiate windowclass");
	
	return hwnd;
}


lib::win::WindowHandle::WindowHandle(HWND handle, bool owned) : handle(handle), owned(owned) {
}

lib::win::WindowHandle::~WindowHandle() {
	if(handle && owned) {
		CloseWindow(handle);
		DestroyWindow(handle);
	}
}

lib::win::WindowHandle::operator HWND() const {
	return handle;
}

void lib::win::WindowHandle::Create(LPCSTR cls, HWND parent, UINT id, DWORD style, DWORD stylex,
		LPCSTR title, int x, int y, int w, int h, HINSTANCE instance) {
	if(handle)
		throw WinError("WindowHandle: window already exists");
	owned = true;
	if(!instance)
		instance = GetModuleHandle(NULL);
	handle = CreateWindowEx(stylex, cls, title, WS_CHILD|style, x, y, w, h, parent, (HMENU)id, instance, NULL);
	if(!handle)
		throw WinError("WindowHandle: unable to create window");
}

void lib::win::WindowHandle::Create(const lib::win::WindowClass & wc, LPCSTR title, DWORD style, DWORD stylex, int x, int y, int w, int h) {
	if(handle)
		throw WinError("WindowHandle: window already exists");
	owned = true;
	handle = wc.Instantiate(title, style, stylex, x, y, w, h);
	if(!handle)
		throw WinError("WindowHandle: unable to create window");
}

HWND lib::win::WindowHandle::Detach() {
	owned = false;
	return handle;
}


void lib::win::debug::out(const char * format, ...) {
	char buffer[1024] = {0};
	va_list list;
	va_start(list, format);
	_vsnprintf(buffer, sizeof(buffer), format, list);
	va_end(list);
	OutputDebugString(buffer);
}

void lib::win::debug::printError(DWORD error) {
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0,
		NULL
	);
	OutputDebugString(static_cast<LPTSTR>(lpMsgBuf));
	LocalFree(lpMsgBuf);
}

void lib::win::debug::printError() {
	lib::win::debug::printError(GetLastError());
}
