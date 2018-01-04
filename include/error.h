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
		if(format[0]) {
			str[len++] = '(';
			len += vsprintf(str + len, format, args);
			str[len++] = ')';
		}
		str[len] = '\0';
	}
public:
	virtual const char *what() const noexcept override { return str; }
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

ERRTYPE(MULTIPLE_DEFAULT_TASK_SET, "Multiple default task detected");

ERRTYPE(JSON_PARSE_FAILED, "Couldn't parse file");
ERRTYPE(FILE_OPEN_FAILED, "Couldn't open file");
ERRTYPE(MULTIPLE_OUTPUT_FILE_SET, "Multiple output file set");

ERRTYPE(MODULE_NOT_FOUND, "Couldn't find module");
ERRTYPE(MODULE_NAME_CONFLICT, "Module name conflicted");
ERRTYPE(MODULE_CYCLIC_DEPENDENCE_FOUND, "Module cyclic dependence found");
ERRTYPE(MODULE_NAME_ILLEGAL, "Illegal module name");
ERRTYPE(MODULE_TYPE_UNDEFINED, "Undefined module type");
ERRTYPE(SOURCE_DEPENDENCE_ANALYSIS_FAILED, "Couldn't analysis file dependence");

ERRTYPE(OBJECT_TYPE_UNSUPPORTED, "Unsupported object type");
ERRTYPE(OBJECT_INVALID, "Invalid object");

ERRTYPE(SWITCHER_EXPRESSION_EVALUATE_FAILED, "Expression evaluate failed");
ERRTYPE(SWITCHER_MULTIPLE_DEFAULT_LABEL, "Multiple default label detected");
ERRTYPE(SWITCHER_MULTIPLE_MATCHED_LABEL, "Multiple matched label detected");
ERRTYPE(SWITCHER_MATCH_FAILED, "Couln't match a label");
}

#endif