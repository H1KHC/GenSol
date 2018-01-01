#ifndef __TARGET_H__
#define __TARGET_H__
#include <vector>
#include <map>
#include "modules/basicModule.h"

struct Target : public basicModule {
	bool generated, fileMatched, fileGenerated;
	std::vector<std::string> sources, sourcesR;
	module<Config, Configs, &configs> config;
	module<Compiler, Compilers, &compilers> compiler;
	module<Linker, Linkers, &linkers> linker;
	void access();
	void parse();
	void loadData(const Object*obj);
	void matchFiles();
	void generate();
	void generateSources();
	std::string compilerCommand(const std::string& src) {
		return compiler.ptr->command(src);
	}
	std::string linkerCommand(const std::string& src) {
		return linker.ptr->command(src, "%@");
	}
	Target(const Object* obj) :
		basicModule(obj), generated(false),
		fileMatched(false), fileGenerated(false) {}
};

struct Targets {
	std::map<std::string, Target*> map;
	template <class T> void parse(const T& obj);
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	Target* find(const std::string& name) {
		auto it = map.find(name);
		if(it == map.end()) throw ERR::MODULE_NOT_FOUND;
		return it->second;
		return nullptr;
	}
	void insert(const Object* obj) {
		Target *target = new Target(obj);
		if(exist(target->name))
			throw ERR::MODULE_NAME_CONFLICT;
		map[target->name] = target;
		trace(ATTR(GREEN) "Found "
			ATTR(RESET)	"target %s",
			target->name.c_str());
	}
	std::map<std::string, Target*>::iterator begin() { return map.begin(); }
	std::map<std::string, Target*>::iterator end() { return map.end(); }
};

extern Targets targets;
#endif