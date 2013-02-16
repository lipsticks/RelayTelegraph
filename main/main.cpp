#include "lib/BabasChessPlugin.h"
#include <stdio.h>

// ------------------------------------------------------------
// Global variables

#define BCPEXP extern "C" __declspec(dllexport)

BABASCHESS_DISPATCH_V_1_4 *pBCDT = NULL;
DWORD dwMyID = 0;

void Out(const char * format, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	OutputDebugString(buffer);
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID) {
	switch(reason)
	{
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
		"&About..."
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
			OutputDebugString("hello!");
			break;
		
		case 1:
			MessageBox(pBCDT->BCAPI_GetMainWnd(), "So what is this about?","About", MB_OK|MB_ICONINFORMATION);
			break;
	}
}

BCPEXP void _cdecl BCP_OnLogOn() {
}

BCPEXP void _cdecl BCP_OnLogOff(BOOL bReason) {
}

BCPEXP void _cdecl BCP_OnTimer() {
}

BCPEXP BOOL _cdecl BCP_OnQTell(ServerOuputQTellType soq,const char *line) {
	Out("QTELL: %s", line);
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
	return FALSE;
}

BCPEXP HWND _cdecl BCP_CreateInfoTabWindow(const GUID *pGUID,HWND hWndParent) {
	return NULL;
}


