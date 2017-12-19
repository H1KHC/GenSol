#include <cstdio>
#include <cstdlib>
#include "error.h"
#include "trace.h"

static char notice[][64] = {
	"Invalid option",
	"Multiple output file set",
	"File not found",
	"Json parse failed",
	"Unsupported object name",
	"Unsupported object format",
	"Undefined module type",
	"Module name conflict",
	"Module not found",
	"Module cyclic dependence found",
	"Could not analysis file's dependence",
	"Multiple default task set"
};
static char buf[16384];
void setError(ERR err, const char *extraInfo, ...) {
	va_list args;
	va_start(args, extraInfo);
	vsprintf(buf, extraInfo, args);
	va_end(args);
	trace(ATTR(RED AND BOLD) "Error %d "
		REATTR(RED)"(%s) %s\n",
		  (int)err + 1, notice[(int)err],
		  buf);
	exit(0);
}