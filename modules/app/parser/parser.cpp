#include "parser.h"

//NOTE: Due to the unavilability of non-capturing groups, this pattern will create a dummy group
//  in the last register slot which should simply be ignored.
const char * app::parser::ScheduleParser::pattern = ":| *\\([0-9]\\{4\\}-[0-9]\\{2\\}-[0-9]\\{2\\}\\) *| *\\(Mon\\|Tue\\|Wed\\|Thu\\|Fri\\|Sat\\|Sun\\) *| *\\([0-9]\\{2\\}:[0-9]\\{2\\}\\) *| *\\([0-9]+\\|LR\\|TB\\|??\\|N/A\\) *| *\\(\\( *[0-9a-zA-Z]\\)+\\) *|.*";


const char * app::parser::ScheduleTableBorderParser::pattern = ":\\+\\(-+\\+\\)\\{5\\}";


//NOTE: The first version of the regular expression was more general but had a dummy group
//  (group #3) due to the unavailability of non-capturing (i.e. "shy") groups. Therefore I
//  wrote a second version of the regex which, however, must be considered incomplete (i.e.
//  bound to fail in some cases) because it makes more assumptions about the input text which
//  I as yet have incomplete knowledge of. This is to say that the regex so far expects
//  group #2 only to match either "Round X" (where X is a number) or "Last Round" or "Tie-Break".
//  If there is any other possibility, it will fail.
//TODO: Find out the exact grammar of the output to the "tell relay listTourney" command.
//const char * RelayTourneyListParser::pattern = ":\\([0-9]+\\) *\\(.+\\) +- +\\(\\( *[0-9a-zA-Z]\\)+\\)  +\\(Open\\|Round Started\\|Round Over\\)";
const char * app::parser::RelayTourneyListParser::pattern = ":\\([0-9]+\\) *\\(.+\\) +- +\\(Round +\\([0-9]+\\)\\|Last Round\\|Tie-Break\\)  +\\(Open\\|Round Started\\|Round Over\\)";


app::parser::ServerDate::ServerDate() : Y(0), M(0), D(0), h(0), m(0) {
}

app::parser::ServerDate::ServerDate(int Y, int M, int D, int h, int m) : Y(Y), M(M), D(D), h(h), m(m) {
}

app::parser::ServerDate::ServerDate(const char * date, const char * time) : h(0), m(0) {
	if(!date) throw std::runtime_error("ServerDate() expected valid 'date' string.");
	lib::parser::Splitter d(date, "-");
	Y = atoi(d[0]);
	M = atoi(d[1]);
	D = atoi(d[2]);
	if(time && *time) {
		lib::parser::Splitter t(time, ":");
		h = atoi(t[0]);
		m = atoi(t[1]);
	}
}

void app::parser::ServerDate::reset() {
	Y = M = D = h = m = 0;
}

void app::parser::ServerDate::load(const ScheduleParser &p) {
	if(!p.matches()) throw std::runtime_error("ServerDate::load() expected a valid ScheduleParser state.");
	lib::parser::Splitter d(p[0], "-");
	Y = atoi(d[0]);
	M = atoi(d[1]);
	D = atoi(d[2]);
	lib::parser::Splitter t(p[2], ":");
	h = atoi(t[0]);
	m = atoi(t[1]);
}

void app::parser::ServerDate::print(FILE * file) const {
	fprintf(file, "ServerDate: %4d-%02d-%02d %02d:%02d", Y, M, D, h, m);
}


//NOTE: It is assumed here that "0" is never a valid value for "Round" in the text printed by
// relay. This way we can use "0" as a NULL value (e.g. undefined).
app::parser::TourneyId::TourneyId() : round(0), lastRound(false), tieBreak(0) {
}

bool app::parser::TourneyId::equals(const TourneyId &other, bool strict, bool sensitive) const {
	if(round != other.round || lastRound != other.lastRound || tieBreak != other.tieBreak)
		return false;
	typedef int (* Method)(const char *, const char *, size_t len);
	Method method = sensitive ? strncmp : _strnicmp;
	return !method(name.data(), other.name.data(), name.length());
}

void app::parser::TourneyId::reset() {
	name = "";
	round = lastRound = tieBreak = 0;
}

void app::parser::TourneyId::load(const ScheduleParser &p) {
	if(!p.matches()) throw std::runtime_error("TourneyId::load() expected a valid ScheduleParser state.");
	reset();
	name = p[4];
	round = atoi(p[3]);
	if(!round) {
		if(!strcmp("LR", p[3]))
			lastRound = true;
		else if(!strcmp("TB", p[3]))
			tieBreak = true;
	}
}

void app::parser::TourneyId::load(const RelayTourneyListParser &p) {
	if(!p.matches()) throw std::runtime_error("TourneyId::load() expected a valid RelayTourneyListParser state.");
	reset();
	name = p[1];
	round = atoi(p[3]);
	if(!round) {
		if(!strcmp("Last Round", p[2]))
			lastRound = true;
		else if(!strcmp("Tie-Break", p[2]))
			tieBreak = true;
	}
}

void app::parser::TourneyId::print(FILE * file) const {
	fprintf(file, "TourneyId: '%s' (", name.data());
	if(round) fprintf(file, "%d", round);
	else {
		if(lastRound) fprintf(file, "LR");
		else if(tieBreak) fprintf(file, "TB");
	}
	printf(")");
}


app::parser::TourneyStatus::TourneyStatus() : handle(0), roundOpen(false), roundStarted(0), roundOver(0) {
}

void app::parser::TourneyStatus::reset() {
	handle = roundOpen = roundStarted = roundOver = 0;
}

void app::parser::TourneyStatus::load(const RelayTourneyListParser &p) {
	if(!p.matches()) throw std::runtime_error("TourneyStatus::load() expected a valid RelayTourneyListParser state.");
	reset();
	handle = atoi(p[0]);
	if(!strcmp("Open", p[4]))
		roundOpen = true;
	else if(!strcmp("Round Started", p[4]))
		roundStarted = true;
	else if(!strcmp("Round Over", p[4]))
		roundOver = true;
}

void app::parser::TourneyStatus::print(FILE * file) const {
	fprintf(file, "TourneyStatus: '%d' ", handle);
	if(roundOpen) fprintf(file, "OPENING");
	else if(roundStarted) fprintf(file, "RUNNING");
	else if(roundOver) fprintf(file, "CLOSING");
}


app::parser::Tourney::Tourney() : date(0,0,0) {
}
