#ifndef __TRACE_H__
#define __TRACE_H__
#include <string>
#include <cstdarg>
#include <cstdio>

#define ATTR(attr)	"\033[" attr "m"
#define REATTR(attr) "\033[0m" ATTR(attr)
#define AND			";"
#define RESET		"0"
#define BOLD		"1"
#define DARKER		"2"
#define UNDERLINE	"4"
#define BLINK		"5"
#define REV			"7"
#define CLR			"8"
#define RED			"31"
#define GREEN		"32"
#define YELLOW		"33"
#define BLUE		"34"
#define MAGENTA		"35"
#define CYAN		"36"
#define GREY		"37"
#define BRED		"41"
#define BGREEN		"42"
#define BYELLOW		"43"
#define BBLUE		"44"
#define BMAGENTA	"45"
#define BCYAN		"46"
#define BGREY		"47"

class Trace {
	char buf[16384];
	const int tabWidthExp;
	std::string spaces;
	int depth, maxDepth, maxTracing;
	void checkMaxDepth() {
		(maxDepth >= depth) ? 0 :
		(spaces.append(std::string((depth - maxDepth) << tabWidthExp, ' ')),
			maxDepth = depth);
	}
public:
	Trace() : tabWidthExp(1), depth(0), maxDepth(0) {}
	void push() { ++depth; checkMaxDepth(); }
	void pop() { --depth; }
	void setMaxTracingDepth(int dep) { maxTracing = (dep == 0 ? 0x7FFFFFFF : dep); }
	void forceOutput(const char *fmt, ...) {
		if(depth >= maxTracing) depth = maxTracing;
		buf[0] = 0;
		if(fmt[0]) {
			va_list args;
			va_start(args, fmt);
			vsprintf(buf, fmt, args);
			va_end(args);
		}
		fprintf(stderr, "%s%s\n" ATTR(RESET),
			spaces.c_str() + ((maxDepth - depth) << tabWidthExp), buf);
	}
	void operator() (const char *fmt, ...) {
		if(depth >= maxTracing) return;
		buf[0] = 0;
		if(fmt[0]) {
			va_list args;
			va_start(args, fmt);
			vsprintf(buf, fmt, args);
			va_end(args);
		}
		fprintf(stderr, "%s%s\n" ATTR(RESET),
			spaces.c_str() + ((maxDepth - depth) << tabWidthExp), buf);
	}
};

extern Trace trace;
#endif