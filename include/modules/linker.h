#ifndef __LINKER_H__
#define __LINKER_H__
#include "modules/basicModule.h"
#include "trace.h"
#include "error.h"
#include <list>
#include <map>

struct Linker : public basicModule {
	bool linkFlagMerged;
	std::string executableName, outputFlag;
	std::list<std::string> linkFlag;
	void merge(const Linker*);
	void access();
	void parse();
	void loadData(const Object*obj);
	std::string command(const std::string& src, const std::string& out) {
		if(!linkFlagMerged) {
			linkFlagMerged = true;
			if(linkFlag.size())
				linkFlag.front().insert(0, " ");
			else linkFlag.push_back(" ");
			for(auto it = linkFlag.begin(), _end = linkFlag.end();
			  ++it != _end;)
				linkFlag.front().append(" " + *it);
			linkFlag.resize(1);
			outputFlag = " " + outputFlag + " ";
		}
		return executableName + outputFlag + out + " " + src + linkFlag.front();
	}
	Linker(const Object* obj) : basicModule(obj), linkFlagMerged(false) {}
	Linker() : basicModule(), linkFlagMerged(false) {}
};

class Linkers {
	std::map<std::string, Linker*> map;
public:
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	Linker* find(const std::string& name) {
		auto it = map.find(name);
		if(it == map.end())
			throw ERR::MODULE_NOT_FOUND("Name: [linker] %s", name.c_str());
		return it->second;
		return nullptr;
	}
	void insert(const Object* obj) {
		Linker *linker = new Linker(obj);
		if(exist(linker->name))
			throw ERR::MODULE_NAME_CONFLICT("Name: [linker] %s",
				linker->name.c_str());
		map[linker->name] = linker;
		trace.log(ATTR(GREEN) "Found "
			ATTR(RESET)	"linker %s",
			linker->name.c_str());
	}
};

extern Linkers linkers;
#endif