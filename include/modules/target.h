#ifndef __TARGET_H__
#define __TARGET_H__
#include <list>
#include <map>
#include "modules/basicModule.h"
#include "trace.h"
#include "error.h"

struct Target : public basicModule {
	bool generated, fileMatched, fileGenerated, needInstall;
	std::list<std::string> sources, sourcesR,
						   installHeaders, installHeadersR;
	std::string installPrefix, headerPrefix;
	Config config;
	Compiler compiler;
	Linker linker;
	void access();
	void parse();
	void loadData(const Object*obj);
	void matchFiles();
	void generate();
	void generateSources();
	Target(const Object* obj) :
		basicModule(obj), generated(false),
		fileMatched(false), fileGenerated(false) {
			config.name = name;
			compiler.name = name;
			linker.name = name;
		}
};

struct Targets {
	std::map<std::string, Target*> map;
	template <class T> void parse(const T& obj);
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	Target* find(const std::string& name) {
		auto it = map.find(name);
		if(it == map.end())
			throw ERR::MODULE_NOT_FOUND("Name: [target] %s", name.c_str());
		return it->second;
		return nullptr;
	}
	void insert(const Object* obj) {
		Target *target = new Target(obj);
		if(exist(target->name))
			throw ERR::MODULE_NAME_CONFLICT("Name: [target] %s",
				target->name.c_str());
		map[target->name] = target;
		trace.log(ATTR(GREEN) "Found "
			ATTR(RESET)	"target %s",
			target->name.c_str());
	}
	std::map<std::string, Target*>::iterator begin() { return map.begin(); }
	std::map<std::string, Target*>::iterator end() { return map.end(); }
};

extern Targets targets;
#endif