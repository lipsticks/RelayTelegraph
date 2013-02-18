#pragma once

#include <stdio.h>
#include <string.h>
#include "../../lib/date/date.h"

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
};

class LocalDate : public Date {
public:
	LocalDate();
	LocalDate(const Date& d);
	LocalDate(const ServerDate& d);
};

} //namespace date

} //namespace app
