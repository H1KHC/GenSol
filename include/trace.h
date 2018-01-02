#ifndef __TRACE_H__
#define __TRACE_H__
#include <string>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <stack>

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
	std::stack<std::string> stack;
	int depth, maxDepth, maxTracing;
	void checkMaxDepth() {
		(maxDepth >= depth) ? 0 :
		(spaces.append(std::string((depth - maxDepth) << tabWidthExp, ' ')),
			maxDepth = depth);
	}
	void output(int dep, const char *fmt, va_list args) {
		buf[0] = 0;
		vsprintf(buf, fmt, args);
		fprintf(stderr, "%s%s\n" ATTR(RESET),
			spaces.c_str() + ((maxDepth - dep) << tabWidthExp), buf);
	}
	friend int main(int, char **);
public:
	Trace() : tabWidthExp(1), depth(0), maxDepth(0) {}
	void push(std::string pos, const char *fmt = "", ...) {
		if(depth < maxTracing && fmt[0]) {
			va_list args;
			va_start(args, fmt);
			output(depth, fmt, args);
			va_end(args);
		}
		++depth; checkMaxDepth();
		stack.push(pos);
	}
	void pop() { --depth; stack.pop(); }
	void setMaxTracingDepth(int dep) { maxTracing = (dep == 0 ? 0x7FFFFFFF : dep); }
	void log(const char *fmt, ...) {
		if(depth >= maxTracing) return;
		va_list args;
		va_start(args, fmt);
		output(depth, fmt, args);
		va_end(args);
	}
	void forceOutput (const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		output((depth >= maxTracing) ? maxTracing : depth, fmt, args);
		va_end(args);
	}
};

extern Trace trace;
#endif