#pragma once
#include <stdio.h>
#include <stdarg.h>

namespace lib {

namespace text {

template<size_t N=1024> class Format {
private:
	const size_t _capacity;
	size_t _length;
	char buffer[N];
public:
	Format() : _capacity(N), _length(0) {
		memset(buffer, 0, N);
	}
	Format(const char * format, ...) : _capacity(N), _length(0) {
		va_list list;
		va_start(list, format);
		set(format, list);
		va_end(list);
	}
	operator const char *() const {
		return buffer;
	}
public:
	Format<N>& set(const char * format, va_list list) {
		_length = _vsnprintf(buffer, N, format, list);
		if(_length < 0) throw std::runtime_error("Format: unknown error while formatting string");
		return *this;
	}
	Format<N>& set(const char * format, ...) {
		va_list list;
		va_start(list, format);
		set(format, list);
		va_end(list);
		return *this;
	}
	const char * get() const {
		return buffer;
	}
	size_t capacity() const {
		return _capacity;
	}
	size_t length() const {
		return _length;
	}
};

} // namespace text

} // namespace lib
