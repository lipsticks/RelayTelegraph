#include "parser.h"

lib::parser::Splitter::Splitter(const char * text, const char * separators, char padding) {
	if(!text || !*text) throw std::runtime_error("Splitter() expected non-empty 'text' string.");
	if(!separators || !*separators) throw std::runtime_error("Splitter() expected non-empty 'separators' string.");
	
	buffer = strdup(text);
	char * tok = strtok(buffer, separators);
	while(tok) {
		if(padding) {
			if(padding == *tok) ++tok;
			strip(&tok, padding);
		}
		lines.push_back(tok);
		tok = strtok(NULL, separators);
	}
}

lib::parser::Splitter::~Splitter() {
	if(buffer) delete buffer;
}

const char * lib::parser::Splitter::operator[](int index) {
	return line(index);
}

void lib::parser::Splitter::strip(char ** strptr, const char symbol) {
	while(symbol == **strptr) ++*strptr;
	int len = strlen(*strptr);
	while(len && symbol == *(*strptr+len-1)) *(*strptr+(--len)) = '\0';
}

const char * lib::parser::Splitter::line(int index) {
	if(index < 0 || index >= lines.size()) return NULL;
	return lines[index];
}

unsigned int lib::parser::Splitter::count() {
	return lines.size();
}


//NOTE: I've selected the _RE_SYNTAX_POSIX_COMMON as the default but it forces me to use lots
//  of escape characters ('\\') in the pattern strings. If I wanted to make this class more
//  usable beyond my limited needs in this current project, I would benefit from reading through
//  the header's list of available syntax flags and select a combination that would approximate
//  a more natural regex flavour.
lib::parser::Regex::Regex(const char * pattern, reg_syntax_t syntax, bool fastMap) : matchedChars(0) {
	memset(&re, 0, sizeof(re_pattern_buffer));
	re_set_syntax(syntax);
	const char * error = re_compile_pattern(pattern, strlen(pattern), &re);
	if(error) throw std::runtime_error(error);
	if(fastMap) {
		re.fastmap = new char[256];
		if(!!re_compile_fastmap(&re))
			throw std::runtime_error("error while generating fastmap");
	}
}

lib::parser::Regex::~Regex() {
	if(re.fastmap) delete[] re.fastmap;
	regfree(&re);
}

const char * lib::parser::Regex::operator[](int index) const {
	return group(index);
}

void lib::parser::Regex::reset() {
	matchedChars = 0;
	for(std::vector<char*>::iterator it=groups.begin(); it!=groups.end(); ++it)
		delete *it;
	groups.clear();
}

bool lib::parser::Regex::match(const char * text, unsigned int offset) {
	reset();
	memset(&regs, 0, sizeof(re_registers));
	matchedChars = re_match(&re, text, strlen(text), offset, &regs);
	if(matchedChars <= 0) return false;
	for(int i=1; i<regs.num_regs-1; ++i) {
		int size = regs.end[i]-regs.start[i];
		char * temp = new char[size+1];
		strncpy(temp, text+regs.start[i], size);
		temp[size] = '\0';
		groups.push_back(temp);
	}
	return true;
}

bool lib::parser::Regex::matches() const {
	return matchedChars > 0;
}

const char * lib::parser::Regex::group(int index) const {
	if(index < 0 || index >= groups.size()) return NULL;
	return groups[index];
}

unsigned int lib::parser::Regex::count() const {
	return groups.size();
}


