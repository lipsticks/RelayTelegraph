#include "win.h"
#include <memory>


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

WNDPROC lib::win::SubclassWindow(HWND h, WNDPROC callback, UINT subclassMessage) {
	WNDPROC original = (WNDPROC)GetWindowLong(h, GWL_WNDPROC);
	SetWindowLong(h, GWL_WNDPROC, (LONG)callback);
	if(subclassMessage) SendMessage(h, subclassMessage, (WPARAM)original, 0);
	return original;
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


void lib::win::ModifyWindowStyles(HWND h, DWORD sadd, DWORD sclr, DWORD xadd, DWORD xclr) {
	if(sadd || sclr) SetWindowLong(h, GWL_STYLE, GetWindowLong(h, GWL_STYLE) & ~sclr | sadd);
	if(xadd || xclr) SetWindowLong(h, GWL_EXSTYLE, GetWindowLong(h, GWL_EXSTYLE) & ~xclr | xadd);
}

void lib::win::SetWindowSize(HWND h, int cx, int cy) {
	SetWindowPos(h, 0, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}

void lib::win::SetWindowLocation(HWND h, int x, int y) {
	SetWindowPos(h, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

UINT lib::win::GetWindowId(HWND h) {
	return GetWindowLong(h, GWL_ID);
}

HWND lib::win::GetRootParent(HWND h) {
	if(NULL == h)
		return NULL;
	HWND parent = h;
	HWND grandparent = GetParent(parent);
	while(grandparent) {
		parent = grandparent;
		grandparent = GetParent(parent);
	}
	return parent;
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


lib::win::Font::Font(bool serif, int height, bool bold, bool proof) : handle(NULL) {
	handle = CreateFont(height, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, proof ? PROOF_QUALITY : DEFAULT_QUALITY, serif ? FF_ROMAN : FF_SWISS, NULL);
	if(!handle) throw WinError(GetLastError(), "Font: unable to create font");
}

lib::win::Font::Font(LPCSTR family, int height, bool bold, bool proof) : handle(NULL) {
	handle = CreateFont(height, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, proof ? PROOF_QUALITY : DEFAULT_QUALITY, FF_DONTCARE, family);
	if(!handle) throw WinError(GetLastError(), "Font: unable to create font");
}

lib::win::Font::~Font() {
	if(handle) DeleteObject(handle);
}

lib::win::Font::operator HFONT() const {
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


void lib::win::list::SetListFonts(HWND h, HFONT body, HFONT head, bool redraw) {
	SendMessage(h, WM_SETFONT, (WPARAM)(HFONT)body, MAKELPARAM((BOOL)redraw, 0));
	SendMessage(ListView_GetHeader(h), WM_SETFONT, (WPARAM)(HFONT)head, MAKELPARAM((BOOL)redraw, 0));
}

void lib::win::list::InsertColumn(HWND h, int col, LPCSTR label, int width) {
	LVCOLUMN c;
	memset(&c, 0, sizeof(c));
	c.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	c.cx = width;
	c.pszText = const_cast<LPSTR>(label);
	c.iSubItem = col;
	ListView_InsertColumn(h, col, &c);
}

int lib::win::list::GetColumnCount(HWND h) {
	return Header_GetItemCount(ListView_GetHeader(h));
}

int lib::win::list::AppendColumn(HWND h, LPCSTR label, int width) {
	int col = GetColumnCount(h);
	InsertColumn(h, col, label, width);
	return col;
}

void lib::win::list::GrowColumnToFit(HWND h, int growableColumn, int visibleColumnCount, bool autoResize) {
	int cols = lib::win::list::GetColumnCount(h);
	if(!visibleColumnCount) visibleColumnCount = cols;
	if(growableColumn < 0 || growableColumn >= cols)
		throw WinError("GrowColumnToFit: argument 1 must be a valid column index");
	if(visibleColumnCount < 1 || visibleColumnCount > cols)
		throw WinError("GrowColumnToFit: argument 2 must be between 1 and # of columns");
	
	int visibleColumnsWidth = 0;
	
	for(int i=0; i<cols; ++i)
		if(i != growableColumn) {
			if(autoResize)
				ListView_SetColumnWidth(h, i, LVSCW_AUTOSIZE);
			if(i < visibleColumnCount)
				visibleColumnsWidth += ListView_GetColumnWidth(h, i);
		}
	
	RECT r;
	GetClientRect(h, &r);
	ListView_SetColumnWidth(h, growableColumn, r.right - visibleColumnsWidth);
}

int lib::win::list::GetColumnWidth(HWND h, int col) {
	const int cols = lib::win::list::GetColumnCount(h);
	if(col >= 0 && col < cols) return ListView_GetColumnWidth(h, col);
	int width = 0;
	for(int i=0; i<cols; ++i)
		width += ListView_GetColumnWidth(h, i);
	return width;
}

void lib::win::list::InsertItem(HWND h, int row, LPCSTR text) {
	LVITEM i;
	memset(&i, 0, sizeof(i));
	i.mask = LVIF_TEXT | LVIF_PARAM;
	i.iItem = row;
	i.iSubItem = 0;
	i.pszText = const_cast<LPSTR>(text);
	ListView_InsertItem(h, &i);
}

int lib::win::list::AppendItem(HWND h, LPCSTR text) {
	int count = ListView_GetItemCount(h);
	InsertItem(h, count, text);
	return count;
}

bool lib::win::list::SetItemData(HWND h, int row, LPARAM data) {
	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));
	item.mask = LVIF_PARAM;
	item.iItem = row;
	item.lParam = data;
	return TRUE == ListView_SetItem(h, &item);
}

LPARAM lib::win::list::GetItemData(HWND h, int row) {
	LVITEM item;
	memset(&item, 0, sizeof(LVITEM));
	item.mask = LVIF_PARAM;
	item.iItem = row;
	if(!ListView_GetItem(h, &item))
		lib::win::debug::out("! ERROR retrieving data for item %d in list %08X", row, h);
	return item.lParam;
}

void lib::win::list::SetItemText(HWND h, int row, int col, LPCSTR text) {
	ListView_SetItemText(h, row, col, const_cast<LPSTR>(text));
}

bool lib::win::list::GetItemText(HWND h, int row, int col, std::string& text, bool autoGrowCapacity) {
	const int initialBufferSize = MAX_PATH;
	const int growSize = MAX_PATH;
	const int maxRetrievalAttempts = 3;
	
	size_t tempSize = autoGrowCapacity || !text.capacity() ? initialBufferSize : text.capacity();
	std::auto_ptr<char> temp(new char[tempSize]);
	
	bool textRetrievedCompletely = false;
	
	LVITEM it;
	memset(&it, 0, sizeof(LVITEM));
	it.mask = LVIF_TEXT;
	it.iItem = row;
	it.iSubItem = col;
	it.pszText = temp.get();
	it.cchTextMax = tempSize;
	
	if(!autoGrowCapacity) {
		const int length = (int) SendMessage(h, LVM_GETITEMTEXT, row, (LPARAM)&it);
		text.assign(temp.get());
		textRetrievedCompletely = length >= 0 && length+1 < tempSize;
		return textRetrievedCompletely;
	}
	
	for(int i=0; i<maxRetrievalAttempts; ++i) {
		const int length = (int) SendMessage(h, LVM_GETITEMTEXT, row, (LPARAM)&it);
		
		textRetrievedCompletely = length >= 0 && length+1 < tempSize;
		if(textRetrievedCompletely)
			break;
		
		tempSize += growSize;
		temp = std::auto_ptr<char>(new char[tempSize]);
		it.pszText = temp.get();
		it.cchTextMax = tempSize;
	}
	
	if(textRetrievedCompletely)
		text.assign(temp.get());
	
	return textRetrievedCompletely;
}

int lib::win::list::GetFirstSelectedItem(HWND h) {
	return ListView_GetNextItem(h, -1, LVNI_ALL | LVNI_SELECTED);
}

bool lib::win::list::IsItemSelected(HWND h, int i) {
	return LVIS_SELECTED == ListView_GetItemState(h, i, LVIS_SELECTED);
}

bool lib::win::list::IsItemFocused(HWND h, int i) {
	return LVIS_FOCUSED == ListView_GetItemState(h, i, LVIS_FOCUSED);
}

void lib::win::list::SetItemSelectionState(HWND h, int i, bool setFocus) {
	const UINT mask = LVIS_SELECTED | (setFocus ? LVIS_FOCUSED : 0);
	ListView_SetItemState(h, i, mask, mask);
}

void lib::win::list::ToggleItemSelectionState(HWND h, int i) {
	UINT flags = lib::win::list::IsItemSelected(h, i) ? 0 : LVIS_SELECTED;
	ListView_SetItemState(h, i, flags, LVIS_SELECTED);
}

void lib::win::list::ClearItemSelectionState(HWND h, bool clearFocus) {
	const UINT mask = LVIS_SELECTED | (clearFocus ? LVIS_FOCUSED : 0);
	int i = ListView_GetNextItem(h, -1, LVNI_ALL | mask);
	while(INVALID_ITEM != i) {
		ListView_SetItemState(h, i, 0, mask);
		i = ListView_GetNextItem(h, i, LVNI_BELOW | mask);
	}
}

bool lib::win::list::EnsureItemVisible(HWND h, int row) {
	return TRUE == ListView_EnsureVisible(h, row, FALSE);
}


