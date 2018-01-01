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
inline void exposeIntoVector(const js::GenericValue<js::UTF8<> >& val,
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

void Config::parse() {
	if(parsed) return;
	parsed = true;
	trace(ATTR(GREEN)	"Parsing "
		ATTR(RESET) "config %s...", name.c_str());
	trace.push();
	#ifdef _DEBUG
	printObject(*obj);
	#endif
	if(obj->HasMember("includeDir"))
		exposeIntoVector((*obj)["includeDir"], includeDir);
	if(obj->HasMember("distDir"))
		getString((*obj)["distDir"], distDir);
	if(obj->HasMember("srcDir"))
		exposeIntoVector((*obj)["srcDir"], srcDir);
	trace.pop();
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
	if(obj->HasMember("executableName"))
		getString((*obj)["executableName"], executableName);
	if(obj->HasMember("compileFlag"))
		exposeIntoVector((*obj)["compileFlag"], compileFlag);
	trace.pop();
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
	if(obj->HasMember("executableName"))
		getString((*obj)["executableName"], executableName);
	if(obj->HasMember("outputFlag"))
		getString((*obj)["outputFlag"], outputFlag);
	if(obj->HasMember("linkFlag"))
		exposeIntoVector((*obj)["linkFlag"], linkFlag);
	trace.pop();
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
	if(obj->HasMember("src"))
		exposeIntoVector((*obj)["src"], sources);
	if(obj->HasMember("srcR"))
		exposeIntoVector((*obj)["srcR"], sourcesR);
	if(obj->HasMember("config"))
		getString((*obj)["config"], config);
	else config = "global";
	if(obj->HasMember("compiler"))
		getString((*obj)["compiler"], compiler);
	else compiler = "global";
	if(obj->HasMember("linker"))
		getString((*obj)["linker"], linker);
	else linker = "global";
	trace.pop();
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
	if(obj->HasMember("target"))
		exposeIntoVector((*obj)["target"], target);
	else target.push_back(module<Target, Targets, &targets>("global"));
	if(obj->HasMember("default")) {
		auto& o = (*obj)["default"];
		if(o.GetType() != js::kTrueType)
			trace(ATTR(YELLOW) "Ignored illegal key 'default'");
		else {
			solution.setDefaultTask(name);
		}
	}
	trace.pop();
}