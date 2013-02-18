#pragma once

//#define STRICT
#define WIN32_LEAN_AND_MEAN
//#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0601
#include <windows.h>
#include <stdarg.h>
#include <stdexcept>
#include <commctrl.h>
#include <stdio.h>
//#include <gdiplus/gdiplus.h>

#define EXCEPTION_BASE(base, name)\
	class name : public base { protected: const char buffer[1024]; int result;\
		public: name(int result=-1, const char * msg=#name) : buffer(), base(msg), result(result) {}\
		public: name(const char * msg) : buffer(), base(msg), result(-1) {}\
		public: virtual const char * what() const throw() {\
			static char buffer[1024];\
			_snprintf(buffer, sizeof(buffer), "[%08X (%d)]: %s", result, result, base::what());\
			return buffer; }\
		};

#define EXCEPTION(base, name)\
	class name : public base {\
		public: name(int result=-1, const char * msg=#name) : base(result, msg) {}\
		public: name(const char * msg) : base(msg) {} };


EXCEPTION_BASE(std::runtime_error, WinError)

namespace lib {

namespace win {

void InitSpecificCommonControls(DWORD flags);

BOOL CreateSimpleWindowClass(WNDCLASSEX &wc, LPCSTR name, WNDPROC=NULL, HBRUSH=NULL, HICON=NULL, HINSTANCE=NULL);

BOOL UnregisterWindowClass(const WNDCLASSEX &wc);

HWND InstantiateWindowClass(const WNDCLASSEX &wc, LPCSTR title, DWORD style=WS_OVERLAPPEDWINDOW, DWORD stylex=0, int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int w=400, int h=300);

int EnterMessageLoop();

void SetWindowSize(HWND h, int cx, int cy);


class WindowClass {
protected:
	WNDCLASSEX wc;
	ATOM handle;

public:
	WindowClass(LPCSTR name, WNDPROC=NULL, HBRUSH brush=NULL, HICON=NULL, HINSTANCE=NULL);
	virtual ~WindowClass();

public:
	BOOL IsRegistered() const;
	HWND Instantiate(LPCSTR title, DWORD style=WS_OVERLAPPEDWINDOW, DWORD stylex=0, int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int w=400, int h=300) const;
};


class WindowHandle {
protected:
	HWND handle;
	bool owned;
public:
	WindowHandle(HWND handle=NULL, bool owned=false);
	virtual ~WindowHandle();
	operator HWND() const;
public:
	virtual void Create(LPCSTR cls, HWND parent, UINT id, DWORD style=WS_VISIBLE, DWORD stlyex=0,
		LPCSTR title="", int x=0, int y=0, int w=100, int h=100, HINSTANCE instance=NULL);
	virtual void Create(const WindowClass & wc, LPCSTR title="", DWORD style=WS_VISIBLE, DWORD stlyex=0,
		int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int w=CW_USEDEFAULT, int h=CW_USEDEFAULT);
	HWND Detach();
};

namespace debug {

void out(const char * format, ...);

void printError(DWORD error);
void printError();

} // namespace debug

} //namespace win

} //namespace lib
