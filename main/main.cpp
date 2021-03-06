#include "../modules/extern/sdk/BabasChessPlugin.h"
#include <stdio.h>
#include "../modules/lib/win/win.h"
#include "../modules/app/parser/parser.h"
#include "../controls/mainPane/mainPane.h"
#include "resource.h"

// ------------------------------------------------------------
// Global variables

#define BCPEXP extern "C" __declspec(dllexport)

BABASCHESS_DISPATCH_V_1_4 *pBCDT = NULL;
DWORD dwMyID = 0;
static const GUID MainPaneGUID = { 0xd287e188, 0x31bf, 0x4c52, { 0x83, 0x70, 0x40, 0xb0, 0xb, 0xee, 0x4a, 0x3b } }; // {D287E188-31BF-4c52-8370-40B00BEE4A3B}
const char MainPaneTabTitle[]="RelayTelegraph";

lib::win::WindowClass wcmw("MAIN_PANE_CLASS", MainPaneProc);
lib::win::WindowHandle hwnd;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID) {
	//NOTE: Apparently this is never called.
	switch(reason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hInstance);
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

BCPEXP BOOL _cdecl BCP_Initialize(DWORD dwBabasChessVersion,DWORD dwPluginID,DWORD *pDispatchversionRequired) {
	if (dwBabasChessVersion<BCP_MAKEVERSION(3,9))
		return FALSE; // this plugin requires BabasChess v3.9 or later

	*pDispatchversionRequired = BCP_MAKEVERSION(1,4);	// Requires dispatch table v1.4

	// Saves the plugin ID
	dwMyID = dwPluginID;
	
	return TRUE;
}

BCPEXP void _cdecl BCP_DeInitialize() {
}

BCPEXP void _cdecl BCP_GetDispatchTable(void *pTable) {
	// saves the pointer to the dispatch table
	pBCDT = (BABASCHESS_DISPATCH_V_1_4*)pTable;
}

BCPEXP void _cdecl BCP_SetMenu(DWORD *nMenuItems,const char ***strItems) {
	static const char *items[] = {
		"&Test",
		"&About...",
	};

	*strItems = items;
	*nMenuItems = sizeof(items)/sizeof(items[0]);
}

BCPEXP BOOL _cdecl BCP_OnUpdateMenuStatus(DWORD nItem,BOOL *bChecked) {
	return TRUE;
}

BCPEXP void _cdecl BCP_OnMenuItem(DWORD nItem) {
	switch(nItem) {
		case 0:
			lib::win::debug::out("hello!");
			break;
		
		case 1:
			MessageBox(pBCDT->BCAPI_GetMainWnd(), "So what is this about?","About", MB_OK|MB_ICONINFORMATION);
			break;
	}
}

BCPEXP void _cdecl BCP_OnLogOn() {
	//NOTE: Maybe we should start a timer here to add a short cushioning delay.
	PostMessage(hwnd, UM_MAINPANE_UPDATE, 0, 0);
}

BCPEXP void _cdecl BCP_OnLogOff(BOOL bReason) {
}

BCPEXP void _cdecl BCP_OnTimer() {
}

BCPEXP BOOL _cdecl BCP_OnQTell(ServerOuputQTellType soq,const char *line) {
	DWORD qm = 0;
	qm = SendMessage(hwnd, UM_MAINPANE_GET_QUERYMODE, 0, 0);
	if(qm) {
		SendMessage(hwnd, UM_MAINPANE_ADD_QTELL_LINE, 0, (LPARAM)line);
		return FALSE;
	}
	return TRUE;
}

BCPEXP BOOL _cdecl BCP_OnNotification(ServerOutputNotificationType son,const char *msg, const char *user, INT nGame, const char *fullLine) {
	return TRUE;
}

BCPEXP BOOL _cdecl BCP_OnRequest(ServerOutputRequestType sor,const char *msg, const char *fullLine) {
	return TRUE;
}

BCPEXP BOOL _cdecl BCP_OnChatMessage(ServerOuputChatType soc,const char *user,const char *titles, INT gameNum,INT chNum,const char *msg, const char *fullLine) {
	return TRUE;
}

BCPEXP void _cdecl BCP_OnPlayedGameResult(const char *strGameType,BOOL bRated,const char *strWhite,const char *strBlack,const char *strResult) {
}

BCPEXP void _cdecl BCP_OnRatingChange(const char *strRatingType,DWORD nOldRating,DWORD nNewRating) {	
}

BCPEXP BOOL _cdecl BCP_EnumInfoTabGUIDs(int index,GUID *pGUID,const char **strTitle,HBITMAP *pHBmp) {
	if(0==index) {
		*pGUID = MainPaneGUID;
		*strTitle = MainPaneTabTitle;
		//const HINSTANCE instance = GetModuleHandle(NULL);
		const HINSTANCE instance = GetModuleHandle("RelayTelegraph.BCPlugin");
		//lib::win::debug::out("HINSTANCE: %08X", instance);
		*pHBmp = LoadBitmap(instance, MAKEINTRESOURCE(IDB_TABICON));
		if(!*pHBmp) {
			lib::win::debug::out("RelayTelegraph: error while loading tab icon");
			lib::win::debug::printError();
		}
		return TRUE;
	}
	return FALSE;
}

BCPEXP HWND _cdecl BCP_CreateInfoTabWindow(const GUID *pGUID,HWND hWndParent) {
	hwnd.Create("MAIN_PANE_CLASS", hWndParent, 1001);
	SendMessage(hwnd, UM_MAINPANE_SET_PLUGIN_PTR, dwMyID, (LPARAM)pBCDT);
	return (HWND)hwnd;
}
