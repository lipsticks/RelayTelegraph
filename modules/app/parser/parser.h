#pragma once

#include "../../lib/parser/parser.h"

namespace app {

namespace parser {

class ScheduleParser : public lib::parser::Regex {
private:
	static const char * pattern;
public:
	ScheduleParser() : Regex(pattern) {
	}
};


class ScheduleTableBorderParser : public lib::parser::Regex {
private:
	static const char * pattern;
public:
	ScheduleTableBorderParser() : Regex(pattern) {
	}
};


class RelayTourneyListParser : public lib::parser::Regex {
private:
	static const char * pattern;
public:
	RelayTourneyListParser() : Regex(pattern) {
	}
};


class ServerDate {
public:
	int Y, M, D, h, m;
public:
	ServerDate();
	ServerDate(int Y, int M, int D, int h=0, int m=0);
	ServerDate(const char * date, const char * time=NULL);
public:
	void reset();
	void load(const ScheduleParser &p);
	void print(FILE * file=stdout) const;
};


class TourneyId {
public:
	std::string name;
	int round;
	bool lastRound, tieBreak;
public:
	TourneyId();
public:
	bool equals(const TourneyId &other, bool strict=false, bool sensitive=false) const;
	void reset();
	void load(const ScheduleParser &p);
	void load(const RelayTourneyListParser &p);
	void print(FILE * file=stdout) const;
};


class TourneyStatus {
public:
	int handle;
	bool roundOpen, roundStarted, roundOver;
public:
	TourneyStatus();
public:
	void reset();
	void load(const RelayTourneyListParser &p);
	void print(FILE * file=stdout) const;
};


class Tourney {
public:
	ServerDate date;
	TourneyId id;
	TourneyStatus status;
public:
	Tourney();
};


} //namespace parser

} //namespace app
