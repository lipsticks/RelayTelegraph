#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include "../../extern/regex/regex.h"
#pragma comment(lib, "../../extern/regex.lib")

//TODO: Maybe stop using the (antiquated) C stdlib and start using Boost?

namespace lib {

namespace parser {

class Splitter {
private:
	char * buffer;
	std::vector<const char *> lines;
public:
	Splitter(const char * text, const char * separators="\n", char padding='\r');
	virtual ~Splitter();
	const char * operator[](int index);
public:
	static void strip(char ** strptr, const char symbol);
public:
	const char * line(int index);
	unsigned int count();
};


//NOTE: I've selected the _RE_SYNTAX_POSIX_COMMON as the default but it forces me to use lots
//  of escape characters ('\\') in the pattern strings. If I wanted to make this class more
//  usable beyond my limited needs in this current project, I would benefit from reading through
//  the header's list of available syntax flags and select a combination that would approximate
//  a more natural regex flavour.
class Regex {
private:
	re_pattern_buffer re;
	re_registers regs;
	std::vector<char*> groups;
	int matchedChars;
public:
	Regex(const char * pattern, reg_syntax_t syntax=_RE_SYNTAX_POSIX_COMMON, bool fastMap=true);
	virtual ~Regex();
	const char * operator[](int index) const;
public:
	void reset();
	bool match(const char * text, unsigned int offset=0);
	bool matches() const;
	const char * group(int index) const;
	unsigned int count() const;
};

} //namespace parser

} //namespace lib
