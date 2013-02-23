#pragma once
#include "../../../lib/win/win.h"

namespace app {

namespace win {

class TourneyListView : public lib::win::WindowHandle {
private:
	lib::win::Font fontBody, fontHead;
public:
	TourneyListView();
public:
	void HandleNotify();
public:
	void Create(HWND parent, UINT id=-1);
	void Clear();
	void AddTourney(const char * date, const char * time, const char * name, const char * round);
	void FitHeaderToClient();
};

} // namespace win

} // namespace app
