#include "date.h"

//NOTE: I'm pretty sure that this will work, regardless of the CPU's timezone.

void app::date::serverToGmt(struct tm &date) {
	time_t stamp = mktime(&date);
	stamp -= 28800;
	date = *gmtime(&stamp);
}

app::date::Stamp::Stamp() {
	lib::date::initStamp(&stamp);
}

time_t app::date::Stamp::get() const {
	return stamp;
}

void app::date::Stamp::print(FILE * file) const {
	lib::date::printDate(&stamp);
}


app::date::Date::Date() {
	time_t stamp;
	lib::date::initStamp(&stamp);
	lib::date::makeDateFromStamp(&date, &stamp);
}

app::date::Date::Date(const Stamp& s) {
	time_t stamp = s.get();
	lib::date::makeDateFromStamp(&date, &stamp);
}

const struct tm& app::date::Date::get() const {
	return date;
}

void app::date::Date::print(FILE * file) const {
	lib::date::printDate(&date, file);
}


app::date::ServerDate::ServerDate(const Date& d) {
	struct tm temp = d.get();
	lib::date::makeOffsetDate(&date, &temp, 7 * (60 * 60));
}

app::date::ServerDate::ServerDate(const LocalDate& d) {
	struct tm temp = d.get();
	lib::date::makeOffsetDate(&date, &temp, 7 * (60 * 60) - lib::date::getLocalTimezoneOffset());
}

app::date::LocalDate::LocalDate() {
	time_t stamp;  time(&stamp);
	date = *localtime(&stamp);
}

app::date::LocalDate::LocalDate(const Date& d) {
	struct tm temp = d.get();
	time_t t;
	lib::date::makeStampFromDate(&t, &temp);
	date = *localtime(&t);
}

app::date::LocalDate::LocalDate(const ServerDate& d) {
	struct tm temp = d.get();
	time_t t;
	lib::date::makeStampFromDate(&t, &temp);
	t -= 7 * (60 * 60);
	date = *localtime(&t);
}


int main()
{
	app::date::Stamp t;
	t.print();
	printf(" (timestamp)\n");
	
	app::date::Date d2(t);
	d2.print();
	printf(" (gmt from timestamp)\n");
	
	app::date::Date d;
	d.print();
	printf(" (gmt)\n");
	
	app::date::ServerDate s(d);
	s.print();
	printf(" (server)\n");
	
	app::date::LocalDate l(d);
	l.print();
	printf(" (local)\n");
	
	app::date::LocalDate l2(s);
	l2.print();
	printf(" (local from server)\n");
	
	app::date::ServerDate s2(l);
	s2.print();
	printf(" (server from local)\n");
	
	return 0;
}
