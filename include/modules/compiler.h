#ifndef __COMPILER_H__
#define __COMPILER_H__
#include <map>
#include <list>
#include "modules/basicModule.h"

struct Compiler : public basicModule {
	bool compileFlagMerged;
	std::string executableName, outputFlag;
	std::list<std::string> compileFlag;
	void merge(const Compiler*);
	void access();
	void parse();
	void loadData(const Object*obj);
	std::string command(const std::string& src, const std::string& out = "") {
		if(!compileFlagMerged) {
			compileFlagMerged = true;
			if(compileFlag.size())
				compileFlag.front().insert(0, " ");
			else compileFlag.push_back(" ");
			for(auto it = compileFlag.begin(), _end = compileFlag.end();
			  ++it != _end;)
				compileFlag.front().append(" " + *it);
			compileFlag.resize(1);
			outputFlag = " " + outputFlag + " ";
		}
		return executableName + (out.length() ? (outputFlag + out) : "") + " " +
			src + compileFlag.front();
	}
	std::string objectFileName(const std::string& src) {
		return src + "." + name + ".o";
	}
	Compiler(const Object* obj) : basicModule(obj), compileFlagMerged(false) {}
	Compiler() : basicModule(), compileFlagMerged(false) {}
};

class Compilers {
	std::map<std::string, Compiler*> map;
public:
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	Compiler* find(const std::string& name) {
		auto it = map.find(name);
		if(it == map.end())
			throw ERR::MODULE_NOT_FOUND("Name: [compiler] %s", name.c_str());
		return it->second;
		return nullptr;
	}
	void insert(const Object* obj) {
		Compiler *compiler = new Compiler(obj);
		if(exist(compiler->name))
			throw ERR::MODULE_NAME_CONFLICT("Name: [compiler] %s",
				compiler->name.c_str());
		map[compiler->name] = compiler;
		trace.log(ATTR(GREEN) "Found "
			ATTR(RESET)	"compiler %s",
			compiler->name.c_str());
	}
};

extern Compilers compilers;
#endif