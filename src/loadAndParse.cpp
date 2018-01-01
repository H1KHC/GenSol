#include <algorithm>
#include <rapidjson/document.h>
#include <stdio.h>
#include "trace.h"
#include "error.h"
#include "modules/task.h"
#include "solution.h"
namespace js = rapidjson;

js::Document document;

void printObject(const js::GenericValue<js::UTF8<> >& obj);

void Solution::load() {
	if(!inputFiles.size())
		addInput("solution.json");
	trace(ATTR(GREEN)	"Loading "
		ATTR(RESET) "files...");
	trace.push();
	for(const fileNode& file : inputFiles)
		analyse(&file);
	trace.pop();
	trace(ATTR(GREEN) "Done!" ATTR(RESET));
}

void Solution::parseObject(const js::GenericValue<js::UTF8<> >& obj, int ID) {
	if(obj.GetType() != js::kObjectType) throw ERR::OBJECT_TYPE_UNSUPPORTED;
	if(!obj.HasMember("type")) throw ERR::MODULE_TYPE_UNDEFINED;
	const char *type = obj["type"].GetString();
	if(!strcmp(type, "config"))
		configs.insert(&obj);
	else if(!strcmp(type, "compiler"))
		compilers.insert(&obj);
	else if(!strcmp(type, "linker"))
		linkers.insert(&obj);
	else if(!strcmp(type, "target"))
		targets.insert(&obj);
	else if(!strcmp(type, "task"))
		tasks.insert(&obj);
	else throw ERR::MODULE_TYPE_UNDEFINED;
}

void Solution::analyse(const fileNode* file) {
	currentFile = file->name;
	trace(ATTR(GREEN) 	"Opening "
		ATTR(RESET)		"file %s...", currentFile.c_str());
	trace.push();
	char *buf; long long size;
	FILE*fp = fopen(file->name.c_str(), "r");
	if(fp == nullptr) throw ERR::FILE_OPEN_FAILED;
	fseeko64(fp, 0, SEEK_END);
	size = ftello64(fp);
	fseeko64(fp, 0, SEEK_SET);
	buf = new char[size + 1];
	size = fread(buf, sizeof(char), size, fp);
	fclose(fp);
	trace.pop();
	trace(ATTR(GREEN)		"Analysing "
		  ATTR(RESET)		"file %s...", currentFile.c_str());

	trace.push();
	if(document.Parse(buf).HasParseError()) throw ERR::JSON_PARSE_FAILED;
	if(!document.IsArray()) throw ERR::OBJECT_TYPE_UNSUPPORTED;
	for(int i = 0, sz = document.Size(); i < sz; ++i)
		parseObject(document[i], i);
	trace.pop();
	delete[] buf;
}

template<class strType>
inline void exposeStringArrayIntoVector(const js::GenericValue<js::UTF8<> >& val,
							 std::vector<strType>& vec) {
	switch(val.GetType()) {
		case js::kStringType:
			vec.push_back(strType(val.GetString()));
			break;
		case js::kArrayType:
			for(int i = 0, sz = val.Size(); i < sz; ++i) {
				auto& f = val[i];
				if(f.GetType() != js::kStringType)
					throw ERR::OBJECT_TYPE_UNSUPPORTED;
				else vec.push_back(strType(f.GetString()));
			}; break;
		default: throw ERR::OBJECT_TYPE_UNSUPPORTED;
	}
}

template<class T, class strType>
inline void getString(const js::GenericValue<T>& val, strType& str) {
	if(val.GetType() != js::kStringType)
		throw ERR::OBJECT_TYPE_UNSUPPORTED;
	str = val.GetString();
}

inline bool stringMatchIgnoreAlphaCase(const char *str1, const char *str2) {
	while(*str1 && *str2)
		if(tolower(*(str1++)) != tolower(*(str2++))) return false;
	return *str1 == *str2;
}

template <class ModuleType>
void pushSwitcher(ModuleType *module, const Object *obj) {
	trace(ATTR(GREEN) "Found" ATTR(RESET) " a switcher");
	trace.push();
	if(!obj->HasMember("expression") || !obj->HasMember("case"))
		throw ERR::OBJECT_TYPE_UNSUPPORTED;
	auto &e = (*obj)["expression"], &c = (*obj)["case"];
	if(e.GetType() != js::kStringType ||
		c.GetType() != js::kArrayType ||
		!c.Size())
		throw ERR::OBJECT_TYPE_UNSUPPORTED;

	const char *tempstr = e.GetString();
	int explen = strlen(tempstr), size = c.Size(), valuelen;
	char *expression = new char[explen + 14], *value = new char[16384];
	strcpy(expression, tempstr);
	strcat(expression, " 2>"
		#ifdef _WIN32
		"null"
		#else
		"/dev/null"
		#endif
	);
	FILE *pp = popen(expression, "r");
	if(!pp)
		throw ERR::SWITCH_EVALUATE_FAILED;
	valuelen = fread(value, sizeof(char), 16383, pp);
	if(valuelen == 0)
		throw ERR::SWITCH_EVALUATE_FAILED;
	pclose(pp);
#ifdef _DEBUG
	trace(ATTR("30") "Expression : %s", value);
#endif
	int defaultTarget = -1, switchTarget = -1;
	for(int i = 0; i < size; ++i)
		if(c[i].GetType() == js::kObjectType) {
			auto& o = c[i];
			if(o.HasMember("default")) {
				if(defaultTarget != -1) //multiple default label
					throw ERR::OBJECT_TYPE_UNSUPPORTED;
				defaultTarget = i;
			} else if(o.HasMember("name")) {
				auto &n = o["name"];
				if(n.GetType() != js::kStringType)
					throw ERR::OBJECT_TYPE_UNSUPPORTED;
				if(stringMatchIgnoreAlphaCase(n.GetString(), value)) {
					if(switchTarget != -1) //multiple matched label
						throw ERR::OBJECT_TYPE_UNSUPPORTED;
					switchTarget = i;
				}
			}
		} else throw ERR::OBJECT_TYPE_UNSUPPORTED;
	if(switchTarget == -1) {
		if(defaultTarget == -1)
			trace(ATTR(YELLOW) "Warning: no matched label!");
		else {
			trace(ATTR(GREEN) "Matched "
				  ATTR(RESET) "default label");
			module->loadData(&c[defaultTarget]);
		}
	} else {
		trace(ATTR(GREEN) "Matched "
			ATTR(RESET) "label #%d", switchTarget);
		module->loadData(&c[switchTarget]);
	}
	//switch
	//default check
	delete expression;
	delete value;
	trace.pop();
}

template <class ModuleType, class ObjectType>
void checkSwitcher(ModuleType *module, ObjectType *object) {
	if(object->HasMember("switcher")) {
		const Object &obj = (*object)["switcher"];
		switch(obj.GetType()) {
			case js::kObjectType: pushSwitcher(module, &obj); break;
			case js::kArrayType:
				for(int i = 0, sz = obj.Size(); i < sz; ++i)
					pushSwitcher(module, &obj[i]);
				break;
			default:
				throw ERR::OBJECT_TYPE_UNSUPPORTED;
		}
	}
}

void Config::loadData(const Object *obj) {
	if(obj->HasMember("includeDir"))
		exposeStringArrayIntoVector((*obj)["includeDir"], includeDir);
	if(obj->HasMember("distDir"))
		getString((*obj)["distDir"], distDir);
	if(obj->HasMember("srcDir"))
		exposeStringArrayIntoVector((*obj)["srcDir"], srcDir);
}

void Config::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		ATTR(RESET) "config %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Compiler::loadData(const Object *obj) {
	if(obj->HasMember("executableName"))
		getString((*obj)["executableName"], executableName);
	if(obj->HasMember("compileFlag"))
		exposeStringArrayIntoVector((*obj)["compileFlag"], compileFlag);
}

void Compiler::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		ATTR(RESET) "compiler %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
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
		exposeStringArrayIntoVector((*obj)["linkFlag"], linkFlag);
}

void Linker::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		ATTR(RESET) "linker %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Target::loadData(const Object *obj) {
	if(obj->HasMember("src"))
		exposeStringArrayIntoVector((*obj)["src"], sources);
	if(obj->HasMember("srcR"))
		exposeStringArrayIntoVector((*obj)["srcR"], sourcesR);
	if(obj->HasMember("config"))
		getString((*obj)["config"], config);
	else config = "global";
	if(obj->HasMember("compiler"))
		getString((*obj)["compiler"], compiler);
	else compiler = "global";
	if(obj->HasMember("linker"))
		getString((*obj)["linker"], linker);
	else linker = "global";
}

void Target::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		  ATTR(RESET)	"target %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}

void Task::loadData(const Object * obj) {
	if(obj->HasMember("target"))
		exposeStringArrayIntoVector((*obj)["target"], target);
	else target.push_back(module<Target, Targets, &targets>("global"));
	if(obj->HasMember("default"))
			solution.setDefaultTask(name);
		}

void Task::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		ATTR(RESET) "task %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
	loadData(obj);
	checkSwitcher(this, obj);
	trace.pop();
}