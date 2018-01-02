#ifndef __ERROR_H__
#define __ERROR_H__
#include <exception>
#include <cstdio>
#include <cstdarg>

namespace ERR {
class ERR : public std::exception {
	char str[1024];
protected:
	void set(const char *info, const char *format, va_list args) {
		int len = 0;
		while(*info) str[len++] = *(info++);
		str[len++] = '(';
		vsprintf(str + len + 1, format, args);
		str[len++] = ')';
	}
public:
	virtual const char *what() { return str; }
};

#define ERRTYPE(NAME, INFO) \
class NAME : public ERR { \
public: \
	NAME(const char *format = "", ...) { \
		va_list args; \
		va_start(args, format); \
		set(INFO, format, args); \
		va_end(args); \
	} \
}

ERRTYPE(MODULE_NOT_FOUND, "Couldn't find module");
ERRTYPE(MODULE_NAME_CONFLICT, "Module name conflicted");\
ERRTYPE(MULTIPLE_DEFAULT_TASK_SET, "Multiple default task detected");
ERRTYPE(FILE_OPEN_FAILED, "Couldn't open file");
ERRTYPE(MODULE_CYCLIC_DEPENDENCE_FOUND, "Module cyclic dependence found");
ERRTYPE(SOURCE_DEPENDENCE_ANALYSIS_FAILED, "Couldn't analysis file dependence");
ERRTYPE(OBJECT_TYPE_UNSUPPORTED, "Unsupported object type");
ERRTYPE(MODULE_NAME_ILLEGAL, "Illegal module name");
ERRTYPE(MODULE_TYPE_UNDEFINED, "Undefined module type");

}

#endif