#pragma once

#include <windows.h>

#define UM_MAINPANE_BASE				(WM_USER+100)
#define UM_MAINPANE_SET_PLUGIN_PTR		(UM_MAINPANE_BASE+1)
#define UM_MAINPANE_ADD_QTELL_LINE		(UM_MAINPANE_BASE+2)
#define UM_MAINPANE_GET_QUERYMODE		(UM_MAINPANE_BASE+3)
#define UM_MAINPANE_CLR_QUERYMODE		(UM_MAINPANE_BASE+4)
#define UM_MAINPANE_QUERY_STATUS_UPDATE	(UM_MAINPANE_BASE+10)
#define UM_MAINPANE_UPDATE				(UM_MAINPANE_BASE+11)

enum QueryMode {QM_NONE=0, QM_RELAYINFO, QM_RELAY, QM_MAMER};
enum QueryStatus {QS_NONE=0, QS_RELAYINFO_STARTING, QS_RELAYINFO_PARSING, QS_RELAYINFO_ENDING, QS_RELAYINFO_DONE};

LRESULT CALLBACK MainPaneProc(HWND h, UINT m, WPARAM w, LPARAM l);
