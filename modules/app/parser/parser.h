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


class ScheduleTableEndParser : public lib::parser::Regex {
private:
	static const char * pattern;
public:
	ScheduleTableEndParser() : Regex(pattern) {
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


//NOTE: This was supposed to be a kind of common denominator and a means to
//  correlate the tourney entries between the two different listings ('tell
//  relay listTourney' and 'tell relayInfo show'). However, it turns out that
//  the names aren't guaranteed to be identical between the two lists. If it
//  had only been a truncating, the partial matching should have worked, but
//  it seems that some names will get abbreviated (e.g. by dropping the vowels).
class TourneyId {
public:
	std::string name, sround;
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
