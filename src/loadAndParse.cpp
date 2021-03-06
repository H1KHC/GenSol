#include <algorithm>
#include <rapidjson/document.h>
#include "trace.h"
#include "error.h"
#include "modules/task.h"
#include "solution.h"
#include "variables.h"
namespace js = rapidjson;

js::Document document;

void printObject(const js::GenericValue<js::UTF8<> >& obj);

inline bool matchString(const char *str1, const char *str2) {
	while(*str1 && *str2)
		if(tolower(*(str1++)) != tolower(*(str2++))) return false;
	return *str1 == *str2;
}

template<class strType>
inline void exposeStringArrayIntolist(const js::GenericValue<js::UTF8<> >& val,
							 std::list<strType>& vec);

void evaluateExpression(const Object *obj, char *value) {
	if(!obj->HasMember("expression"))
		throw ERR::OBJECT_INVALID("Missing \"expression\"");
	auto &e = (*obj)["expression"];
	if(e.GetType() != js::kStringType)
		throw ERR::OBJECT_TYPE_UNSUPPORTED("Expected String or Array");

	int valuelen;
	const char *expression = e.GetString();

	FILE *pp = popen(expression, "r");
	if(!pp)
		throw ERR::SWITCHER_EXPRESSION_EVALUATE_FAILED(expression);
	valuelen = fread(value, sizeof(char), 16383, pp);
	if(valuelen == 0)
		throw ERR::SWITCHER_EXPRESSION_EVALUATE_FAILED(expression);
	while(!isprint(value[valuelen - 1])) value[--valuelen] = '\0';
	pclose(pp);
}

template <class ModuleType>
void pushSwitcher(ModuleType *module, const Object *obj) {
	char *expr = new char[16384];
	trace.push("Expression",
		ATTR(GREEN) "Evaluating"
		ATTR(RESET) " expression");
	evaluateExpression(obj, expr);
	trace.pop();
	trace.verbose("Expression evaluated: %s", expr);
	trace.push("Case labels",
		ATTR(GREEN) "Matching"
		ATTR(RESET) " labels");
	if(!obj->HasMember("case"))
		throw ERR::OBJECT_INVALID("Missing \"case\"");
	auto &c = (*obj)["case"];
	if(c.GetType() != js::kArrayType)
		throw ERR::OBJECT_TYPE_UNSUPPORTED(
			"Expect Array"
		);
	if(!c.Size())
		throw ERR::OBJECT_INVALID("Empty case labels");
	int defaultTarget = -1, switchTarget = -1, size = c.Size();
	for(int i = 0; i < size; ++i) {
		char buf[32];
		sprintf(buf, "Case label #%d", i);
		trace.push(buf);
		if(c[i].GetType() == js::kObjectType) {
			auto& o = c[i];
			if(o.HasMember("default")) {
				if(defaultTarget != -1)
					throw ERR::SWITCHER_MULTIPLE_DEFAULT_LABEL();
				defaultTarget = i;
			} else if(o.HasMember("name")) {
				std::list<std::string> names;
				exposeStringArrayIntolist(o["name"], names);
				for(auto &name : names)
				if(matchString(name.c_str(), expr)) {
					if(switchTarget != -1) //multiple matched label
						throw ERR::SWITCHER_MULTIPLE_MATCHED_LABEL();
					switchTarget = i;
				}
			}
		} else throw ERR::OBJECT_TYPE_UNSUPPORTED(
			"Expeted Object"
		);
		trace.pop();
	}
	if(switchTarget == -1) {
		if(defaultTarget == -1)
			throw ERR::SWITCHER_MATCH_FAILED();
		else {
			trace.log(ATTR(GREEN) "Matched "
					ATTR(RESET) "default label");
			module->loadData(&c[defaultTarget]);
		}
	} else {
		trace.log(ATTR(GREEN) "Matched "
				ATTR(RESET) "label #%d", switchTarget);
		module->loadData(&c[switchTarget]);
	}
	delete expr;
	trace.pop();
}

template <class ModuleType>
void checkSwitcher(ModuleType *module, const Object *object) {
	if(object->HasMember("switcher")) {
		trace.push("Switcher Container",
			ATTR(GREEN) "Found"
			ATTR(RESET) " a switcher container!");
		const Object &obj = (*object)["switcher"];
		switch(obj.GetType()) {
			case js::kObjectType: {
				trace.push("Switcher",
				  ATTR(GREEN) "Handling"
				  ATTR(RESET) " switcher...");
				pushSwitcher(module, &obj);
				trace.pop();
			}break;
			case js::kArrayType:
				for(int i = 0, sz = obj.Size(); i < sz; ++i) {
					char temp[16];
					sprintf(temp, "Switcher #%d", i);
					trace.push(temp,
					ATTR(GREEN) "Handling"
					ATTR(RESET) " switcher #%d...", i);
					pushSwitcher(module, &obj[i]);
				}
				break;
			default:
				throw ERR::OBJECT_TYPE_UNSUPPORTED(
					"Expected Object or Array"
				);
		}
		trace.pop();
	}
}

void Solution::load() {
	if(!inputFiles.size())
		addInput("solution.json");
	trace.push("Solution load",
		ATTR(GREEN) "Loading "
		ATTR(RESET) "files...");
	for(const fileNode& file : inputFiles)
		analyse(&file);
	trace.pop();
	trace.log(ATTR(GREEN) "Done!" ATTR(RESET));
}

void Solution::parseObject(const js::GenericValue<js::UTF8<> >& obj, int ID) {
	if(obj.GetType() != js::kObjectType)
		throw ERR::OBJECT_TYPE_UNSUPPORTED();
	if(!obj.HasMember("type")) throw ERR::MODULE_TYPE_UNDEFINED();
	const char *type = obj["type"].GetString();
	if(matchString(type, "config"))
		configs.insert(&obj);
	else if(matchString(type, "compiler"))
		compilers.insert(&obj);
	else if(matchString(type, "linker"))
		linkers.insert(&obj);
	else if(matchString(type, "target"))
		targets.insert(&obj);
	else if(matchString(type, "task"))
		tasks.insert(&obj);
	// else if(matchString(type, "variables"))
	// 	variables.insert(&obj);
	else throw ERR::MODULE_TYPE_UNDEFINED("type: %d", type);
}

void Solution::analyse(const fileNode* file) {
	currentFile = file->name;
	trace.push("File " + currentFile,
		ATTR(GREEN) "Opening "
		ATTR(RESET) "file %s...", currentFile.c_str());
	char *buf; long long size;
	FILE*fp = fopen(file->name.c_str(), "r");
	if(fp == nullptr) throw ERR::FILE_OPEN_FAILED();
	fseeko64(fp, 0, SEEK_END);
	size = ftello64(fp);
	fseeko64(fp, 0, SEEK_SET);
	buf = new char[size + 1];
	size = fread(buf, sizeof(char), size, fp);
	fclose(fp);
	trace.pop();

	trace.push("File " + currentFile,
		ATTR(GREEN) "Analysing "
		ATTR(RESET) "file %s...", currentFile.c_str());
	if(document.Parse(buf).HasParseError())
		throw ERR::JSON_PARSE_FAILED(
			"Error code: %d", document.GetParseError()
		);
	if(!document.IsArray())
		throw ERR::OBJECT_TYPE_UNSUPPORTED(
			"expected array"
		);
	for(int i = 0, sz = document.Size(); i < sz; ++i) {
		sprintf(buf, "document.#%d", i);
		trace.push(buf);
		parseObject(document[i], i);
		trace.pop();
	}
	trace.pop();
	delete[] buf;
}

template<class strType>
inline void exposeStringArrayIntolist(const js::GenericValue<js::UTF8<> >& val,
							 std::list<strType>& vec) {
	switch(val.GetType()) {
		case js::kStringType:
			vec.push_back(strType(val.GetString()));
			break;
		case js::kArrayType:
			for(int i = 0, sz = val.Size(); i < sz; ++i) {
				auto& f = val[i];
				if(f.GetType() != js::kStringType)
					throw ERR::OBJECT_TYPE_UNSUPPORTED("Expected String");
				else vec.push_back(strType(f.GetString()));
			}; break;
		default: throw ERR::OBJECT_TYPE_UNSUPPORTED("Expected String or Array");
	}
}

template<class T, class strType>
inline void getString(const js::GenericValue<T>& val, strType& str) {
	if(val.GetType() != js::kStringType)
		throw ERR::OBJECT_TYPE_UNSUPPORTED("Expected String");
	str = val.GetString();
}

void Config::loadData(const Object *obj) {
	if(obj->HasMember("includeDir"))
		exposeStringArrayIntolist((*obj)["includeDir"], includeDir);
	if(obj->HasMember("distDir"))
		getString((*obj)["distDir"], distDir);
	if(obj->HasMember("installPrefix"))
		getString((*obj)["installPrefix"], installPrefix);
	if(obj->HasMember("srcDir"))
		exposeStringArrayIntolist((*obj)["srcDir"], srcDir);
	if(obj->HasMember("srcDirR"))
		exposeStringArrayIntolist((*obj)["srcDirR"], srcDirR);
}

void Config::parse() {
	if(parsed) return;
	parsed = true;
	if(!obj) return; // The one in the target doesn't has to be parsed
	trace.push("Config " + name,
		ATTR(GREEN)"Parsing "
		ATTR(RESET)"config %s...", name.c_str());
	printObject(*obj);
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Compiler::loadData(const Object *obj) {
	if(obj->HasMember("executableName"))
		getString((*obj)["executableName"], executableName);
	if(obj->HasMember("outputFlag"))
		getString((*obj)["outputFlag"], outputFlag);
	if(obj->HasMember("compileFlag"))
		exposeStringArrayIntolist((*obj)["compileFlag"], compileFlag);
}

void Compiler::parse() {
	if(parsed) return;
	parsed = true;
	if(!obj) return;
	trace.push("Compiler " + name,
		ATTR(GREEN)"Parsing "
		ATTR(RESET)"compiler %s...", name.c_str());
	printObject(*obj);
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Linker::loadData(const Object*obj) {
	if(obj->HasMember("executableName"))
		getString((*obj)["executableName"], executableName);
	if(obj->HasMember("outputFlag"))
		getString((*obj)["outputFlag"], outputFlag);
	if(obj->HasMember("linkFlag"))
		exposeStringArrayIntolist((*obj)["linkFlag"], linkFlag);
}

void Linker::parse() {
	if(parsed) return;
	parsed = true;
	if(!obj) return;
	trace.push("Linker " + name,
		ATTR(GREEN)"Parsing "
		ATTR(RESET)"linker %s...", name.c_str());
	printObject(*obj);
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Target::loadData(const Object *obj) {
	if(obj->HasMember("src"))
		exposeStringArrayIntolist((*obj)["src"], sources);
	if(obj->HasMember("srcR"))
		exposeStringArrayIntolist((*obj)["srcR"], sourcesR);
	if(obj->HasMember("installHeaders"))
		exposeStringArrayIntolist((*obj)["installHeaders"], installHeaders);
	if(obj->HasMember("installHeadersR"))
		exposeStringArrayIntolist((*obj)["installHeadersR"], installHeadersR);
	if(obj->HasMember("installPrefix"))
		getString((*obj)["installPrefix"], installPrefix);
	if(obj->HasMember("headerPrefix"))
		getString((*obj)["headerPrefix"], headerPrefix);
	if(obj->HasMember("config"))
		exposeStringArrayIntolist((*obj)["config"], config.base);
	if(obj->HasMember("compiler"))
		exposeStringArrayIntolist((*obj)["compiler"], compiler.base);
	if(obj->HasMember("linker"))
		exposeStringArrayIntolist((*obj)["linker"], linker.base);
	if(obj->HasMember("needInstall"))
		needInstall = true;
}

void Target::parse() {
	if(parsed) return;
	parsed = true;
	if(!obj) return;
	trace.push("Target " + name,
		ATTR(GREEN)"Parsing "
		ATTR(RESET)"target %s...", name.c_str());
	printObject(*obj);
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Task::loadData(const Object * obj) {
	if(obj->HasMember("target"))
		exposeStringArrayIntolist((*obj)["target"], target);
	if(obj->HasMember("default"))
		solution.setDefaultTask(name);
}

void Task::parse() {
	if(parsed) return;
	parsed = true;
	if(!obj) return;
	trace.push("Task " + name,
		ATTR(GREEN)"Parsing "
		ATTR(RESET)"task %s...", name.c_str());
	printObject(*obj);
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}
