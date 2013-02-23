#include "mainPane.h"
#include "../../modules/extern/sdk/BabasChessPlugin.h"
#include "../../modules/lib/win/win.h"
#include "../../modules/lib/text/text.h"
#include "../../modules/app/parser/parser.h"
#include "../../modules/app/win/TourneyListView/TourneyListView.h"
#include <string.h>
#include <vector>

class QTellParser {
protected:
	const HWND parent;
public:
	QTellParser(HWND parent=NULL) : parent(parent) {
	}
	virtual ~QTellParser() {
	}
public:
	virtual void reset() = 0;
	virtual bool running() const = 0;
	virtual void addQTellLine(const char * line) = 0;
};

class RelayInfoTourney{
public:
	typedef std::vector<RelayInfoTourney> Vector;
public:
	app::parser::ServerDate d;
	app::parser::TourneyId i;
	std::string date, time, name, round;
public:
	RelayInfoTourney(const app::parser::ScheduleParser& p) {
		d.load(p);
		i.load(p);
		
		lib::text::Format<> f;
		date = f.set("%4d-%02d-%02d", d.Y, d.M, d.D);
		time = f.set("%02d-%02d", d.h, d.m);
		name = i.name;
		round = i.sround;
	}
};

class RelayInfoParser : public QTellParser {
private:
	DWORD status;
	bool statusUpdate;
	app::parser::ScheduleTableBorderParser scheduleTableBorderParser;
	app::parser::ScheduleTableEndParser scheduleTableEndParser;
	app::parser::ScheduleParser scheduleParser;
public:
	RelayInfoTourney::Vector tourneys;
public:
	RelayInfoParser(HWND parent) : QTellParser(parent) {
		statusUpdate = false;
		status = QS_RELAYINFO_DONE;
	}
private:
	void UpdateParsingStatus(const char * line) {
		if(scheduleTableBorderParser.match(line)) {
			statusUpdate = true;
			status = QS_RELAYINFO_STARTING == status ? QS_RELAYINFO_PARSING : QS_RELAYINFO_ENDING;
		}
		else if(scheduleTableEndParser.match(line)) {
			statusUpdate = true;
			status = QS_RELAYINFO_DONE;
		}
		NotifyParent();
	}
	void NotifyParent() {
		if(statusUpdate) {
			statusUpdate = false;
			SendMessage(parent, UM_MAINPANE_QUERY_STATUS_UPDATE, QM_RELAYINFO, status);
		}
	}
public:
	void reset() {
		statusUpdate = true;
		status = QS_RELAYINFO_STARTING;
	}
	bool running() const {
		return QS_RELAYINFO_DONE != status;
	}
	void addQTellLine(const char * line) {
		UpdateParsingStatus(line);
		if(QS_RELAYINFO_PARSING == status) {
			if(scheduleParser.match(line))
				tourneys.push_back(RelayInfoTourney(scheduleParser));
		}
	}
public:
	void fillList(app::win::TourneyListView& tl) const {
		for(int i=0; i<tourneys.size(); ++i) {
			const RelayInfoTourney &t = tourneys[i];
			tl.AddTourney(t.date.data(), t.time.data(), t.name.data(), t.round.data());
		}
		tl.FitHeaderToClient();
	}
};

LRESULT CALLBACK MainPaneProc(HWND h, UINT m, WPARAM w, LPARAM l) {
	enum {ID_NONE=1000,ID_TL};
	static DWORD dwPluginID = 0;
	static BABASCHESS_DISPATCH_V_1_4 *pBCDT = NULL;
	static RECT r = {2, 2, 26, 26};
	static bool on = false;
	static app::win::TourneyListView tl;
	static DWORD queryMode = QM_NONE;
	static RelayInfoParser rip(h);
	
	switch(m) {
		case WM_CREATE: {
			tl.Create(h, ID_TL);
			//tl.AddTourney("2013-02-20", "06:00", "Karkonosze Open Group A", "5");
			//tl.AddTourney("2013-02-21", "00:00", "Cen.Serbia Champ's Men", "5");
			return 0;
		}
		case WM_SIZE: {
			const int cx=LOWORD(l), cy=HIWORD(l);
			const int tlY = 30;
			MoveWindow(tl, 0, tlY, cx, cy-tlY, FALSE);
			
			tl.FitHeaderToClient();
			return 0;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(h, &ps);
			SaveDC(hdc);
			
			COLORREF c = on ? RGB(220,200,180) : RGB(255,255,255);
			HBRUSH hb = CreateSolidBrush(c);
			SelectObject(hdc, hb);
			
			Rectangle(hdc, r.left, r.top, r.right, r.bottom);
			if(!on) {
				const char * message = "<- click here to update list";
				SetBkMode(hdc, TRANSPARENT);
				TEXTMETRIC tm;
				GetTextMetrics(hdc, &tm);
				TextOut(hdc, r.left + r.right + 10, r.top + ((r.bottom-r.top)>>1) - (tm.tmHeight>>1), message, strlen(message));
			}
			
			DeleteObject(hb);
			RestoreDC(hdc, -1);
			EndPaint(h, &ps);
			return 0;
		}
		case WM_LBUTTONUP: {
			POINT p = {LOWORD(l), HIWORD(l)};
			if(PtInRect(&r, p) && !on) {
				on = true;
				InvalidateRect(h, NULL, TRUE);
				UpdateWindow(h);
				
				PostMessage(h, UM_MAINPANE_UPDATE, 0, 0);
			}
			return 0;
		}
		case UM_MAINPANE_SET_PLUGIN_PTR: {
			dwPluginID = w;
			pBCDT = (BABASCHESS_DISPATCH_V_1_4*)l;
			//lib::win::debug::out("plugin ptr: %08X", pBCDT);
			//lib::win::debug::out("main HWND (plugin ptr test): %08X", pBCDT->BCAPI_GetMainWnd());
			return 0;
		}
		case UM_MAINPANE_ADD_QTELL_LINE: {
			const char * line  = (const char *)l;
			//lib::win::debug::out("QTELL: %s", line);
			rip.addQTellLine(line);
			return 0;
		}
		case UM_MAINPANE_GET_QUERYMODE: {
			return queryMode;
		}
		case UM_MAINPANE_CLR_QUERYMODE: {
			queryMode = 0;
			on = false;
			return 0;
		}
		case UM_MAINPANE_QUERY_STATUS_UPDATE: {
			lib::win::debug::out("UM_MAINPANE_QUERY_STATUS_UPDATE: %d %d", w, l);
			
			if(QM_RELAYINFO == w && QS_RELAYINFO_DONE == l) {
				queryMode = 0;	//VERY IMPORTANT! otherwise the plugin will keep discarding
								// QTells intended for output by the console.
				tl.Clear();
				rip.fillList(tl);
				
				for(int i=0; i<rip.tourneys.size(); ++i) {
					const RelayInfoTourney &t = rip.tourneys[i];
					lib::win::debug::out("Scheduled Tounrey #%2d: %s %s %s(%s)", i+1,
						t.date.data(), t.time.data(), t.name.data(), t.round.data());
				}
			}
			return 0;
		}
		case UM_MAINPANE_UPDATE: {
			if(!rip.running()) {
				rip.reset();
				queryMode = QM_RELAYINFO;
				if(pBCDT) pBCDT->BCAPI_SendCommand(dwPluginID, "tell relayInfo show");
			}
			return 0;
		}
	}
	return DefWindowProc(h, m, w, l);
}
