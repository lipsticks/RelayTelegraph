#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

namespace lib {

namespace date {

int getLocalTimezoneOffset();

bool isDaylightSavingsTime();

bool makeOffsetDate(struct tm *o, const struct tm *d, int offsetSeconds=0);

// Timezone-preserving operations.

void initStamp(time_t *t);

bool makeStampFromDate(time_t *t, const struct tm *d);

bool makeDateFromStamp(struct tm *d, const time_t *t);

void printDate(const struct tm *d, FILE * file=stdout);

bool printDate(const time_t *t, FILE * file=stdout);

void printStamp(const time_t *t, FILE * file=stdout);

bool printStamp(const struct tm *d, FILE * file=stdout);

} //namespace date

} //namespace lib
