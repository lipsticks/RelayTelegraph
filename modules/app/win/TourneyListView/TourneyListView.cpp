#include "TourneyListView.h"

app::win::TourneyListView::TourneyListView() : fontBody(false, 18), fontHead(false, 12) {
}

void app::win::TourneyListView::Create(HWND parent, UINT id) {
	lib::win::WindowHandle::Create(WC_LISTVIEW, parent, id, WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL);
	SendMessage(*this, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	lib::win::list::SetListFonts(*this, fontBody, fontHead);
	
	lib::win::list::AppendColumn(*this, "Date", 120);
	lib::win::list::AppendColumn(*this, "Time", 80);
	lib::win::list::AppendColumn(*this, "Round", 50, true);
	lib::win::list::AppendColumn(*this, "Title", 240);
}

void app::win::TourneyListView::Clear() {
	ListView_DeleteAllItems(*this);
}

void app::win::TourneyListView::AddTourney(const char * date, const char * time, const char * name, const char * round) {
	int i = lib::win::list::AppendItem(*this);
	lib::win::list::SetItemText(*this, i, 0, date);
	lib::win::list::SetItemText(*this, i, 1, time);
	lib::win::list::SetItemText(*this, i, 2, round);
	lib::win::list::SetItemText(*this, i, 3, name);
}

void app::win::TourneyListView::FitHeaderToClient() {
	lib::win::list::GrowColumnToFit(*this, 3);
	ShowScrollBar(*this, SB_HORZ, FALSE);
}

