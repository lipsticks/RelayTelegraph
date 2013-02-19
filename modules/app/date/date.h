#pragma once

#include <stdio.h>
#include <string.h>
#include "../../lib/date/date.h"

//NOTE: Maybe we would have simplified things by adding toGmtStamp() and toGmtDate() methods.
//TODO: Add operator<<()'s where absent.

namespace app {

namespace date {

void serverToGmt(struct tm &date);

class Stamp {
protected:
	time_t stamp;
public:
	Stamp();
public:
	time_t get() const;
	void print(FILE * file=stdout) const;
};

class Date {
protected:
	struct tm date;
public:
	Date();
	Date(const Stamp& s);
public:
	const struct tm& get() const;
	void print(FILE * file=stdout) const;
};

class LocalDate;

class ServerDate : public Date {
public:
	ServerDate(const Date& d);
	ServerDate(const LocalDate& d);
	ServerDate(const ServerDate& d);
};

class LocalDate : public Date {
public:
	LocalDate();
	LocalDate(const Date& d);
	LocalDate(const LocalDate& d);
	LocalDate(const ServerDate& d);
};

class AdjustedDate : public Date {
private:
	int offset;
public:
	AdjustedDate(int offset=0);
public:
	AdjustedDate& operator<<(const Date& d);
	AdjustedDate& operator<<(const LocalDate& d);
	AdjustedDate& operator<<(const ServerDate& d);
	AdjustedDate& operator<<(const AdjustedDate& d);
};

} //namespace date

} //namespace app
