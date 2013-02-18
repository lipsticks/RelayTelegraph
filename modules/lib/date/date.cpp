#include "date.h"

int lib::date::getLocalTimezoneOffset() {
	struct _timeb tb;
	_tzset();
	_ftime(&tb);
	return -tb.timezone * 60;
}

bool lib::date::isDaylightSavingsTime() {
	struct _timeb tb;
	_tzset();
	_ftime(&tb);
	return tb.dstflag;
}

bool lib::date::makeOffsetDate(struct tm *o, const struct tm *d, int offsetSeconds) {
	time_t t;
	if(!makeStampFromDate(&t, d)) return false;
	t += offsetSeconds;
	if(!makeDateFromStamp(o, &t)) return false;
	return true;
}

// The following are timezone-preserving operations.

void lib::date::initStamp(time_t *t) {
	time(t);
}

//FIXME: There is an unresolved question concerning the following two
//  conversion functions (makeStampFromDate and makeDateFromStamp)
//  relating to the daylight savings flag (tm_isdst). It seems that
//  what is needed to solve this problem is a function that would
//  take a stamp or date and return whether at that time daylight
//  savings would have been the case, like so:
//bool lib::date::isDaylightSavingsTime(const struct tm *d) {};
//bool lib::date::isDaylightSavingsTime(const time_t *t) {};

//NOTE: mktime will modify the returned timestamp based on the date's
//  tm_isdst (daylight savings) value (0: no, 1: yes, -1:auto). I'm
//  not quite sure what to do here yet. Normally the date 'd' will
//  itself be the result of some time function which would have set
//  this flag correctly. The documentation says that gmtime will 
//  always set this to 0, though.
bool lib::date::makeStampFromDate(time_t *t, const struct tm *d) {
	*t = mktime(const_cast<struct tm*>(d));
	if(static_cast<time_t>(-1) == *t) return false;
	//NOTE: Since mktime will make adjustments based on the local
	//  timezone we need to make compensating corrections here.
	*t += getLocalTimezoneOffset();
	return true;
}

//NOTE: The documentation states that gmtime always sets the DST
//  flag (tm_isdst) to zero (= no daylight savings correction).
//  Shouldn't it instead have set this based on what was the
//  case at the day referenced by the timestamp 't'?
bool lib::date::makeDateFromStamp(struct tm *d, const time_t *t) {
	struct tm * result = gmtime(t);
	*d = *result;
	return result;
}

void lib::date::printDate(const struct tm *d, FILE * file) {
	//NOTE: Since asctime will print a newline after the date
	//  we need to shorten the string to leave it out.
	const char * temp = asctime(d);
	//TODO: Is this really needed? The date string should be
	//  24 characters long in every case.
	size_t len = strlen(temp);
	fprintf(file, "%.*s", len-1, temp);
}

bool lib::date::printDate(const time_t *t, FILE * file) {
	struct tm d;
	if(!makeDateFromStamp(&d, t)) return false;
	printDate(&d, file);
	return true;
}

void lib::date::printStamp(const time_t *t, FILE * file) {
	fprintf(file, "%d", *t);
}

bool lib::date::printStamp(const struct tm *d, FILE * file) {
	time_t t;
	if(!makeStampFromDate(&t, d)) return false;
	printStamp(&t, file);
	return true;
}
