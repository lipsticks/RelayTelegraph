#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <string.h>

//FIXME: This is a quick hack. I'm not really sure whether these methods
//  will work in all timezones, under all circumstances. I'll need to
//  play around with my system's timezone settings and maybe have other
//  people test this code in different timezones and machines.

size_t getLocalTimezoneOffset() {
	struct _timeb tb;
	_tzset();
	_ftime(&tb);
	return tb.timezone * 60;
}

void serverToGmt(struct tm &date) {
	time_t stamp = mktime(&date);
	stamp -= 28800;
	date = *gmtime(&stamp);
}

class Stamp {
protected:
	time_t stamp;
public:
	Stamp() {
		time(&stamp);
	}
public:
	time_t get() const {
		return stamp;
	}
	void print(FILE * file=stdout) const {
		fprintf(stdout, "Stamp: %d", stamp);
	}
};

class Date {
protected:
	struct tm date;
public:
	Date() {
		time_t stamp;
		time(&stamp);
		date = *gmtime(&stamp);
	}
	Date(const Stamp& s) {
		time_t stamp = s.get();
		date = *gmtime(&stamp);
	}
public:
	const struct tm& get() const {
		return date;
	}
	void print(FILE * file=stdout) const {
		const char * str = asctime(&date);
		size_t len = strlen(str);
		fprintf(file, "%.*s", len-1, str);
	}
};

class ServerDate : public Date {
public:
	ServerDate(const Date& d) {
		struct tm temp = d.get();
		time_t stamp = mktime(&temp);
		stamp -= 25200;
		date = *gmtime(&stamp);
	}
};

class LocalDate : public Date {
public:
	LocalDate() {
		time_t stamp;  time(&stamp);
		date = *localtime(&stamp);
	}
	LocalDate(const Date& d) {
		struct tm temp = d.get();
		//temp.tm_isdst = 0;
		time_t stamp = mktime(&temp);
		stamp += -getLocalTimezoneOffset() * 2;
		date = *gmtime(&stamp);
	}
};

int main()
{
	Stamp t;
	t.print();
	printf("        (timestamp)\n");
	
	Date d2(t);
	d2.print();
	printf(" (gmt from timestamp)\n");
	
	Date d;
	d.print();
	printf(" (gmt)\n");
	
	ServerDate s(d);
	s.print();
	printf(" (server)\n");
	
	LocalDate l(d);
	l.print();
	printf(" (local)\n");

	return 0;
}
