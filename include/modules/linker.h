#ifndef __LINKER_H__
#define __LINKER_H__
#include <vector>
#include <map>
#include "modules/basicModule.h"

struct Linker : public basicModule {
	bool linkFlagMerged;
	std::string executableName, outputFlag;
	std::vector<std::string> linkFlag;
	void merge(const Linker*);
	void access();
	void parse();
	std::string command(const std::string& src, const std::string& out) {
		if(!linkFlagMerged) {
			linkFlagMerged = true;
			if(linkFlag.size())
				linkFlag[0].insert(0, " ");
			else linkFlag.push_back(" ");
			for(int i = 1, sz = linkFlag.size(); i < sz; ++i)
				linkFlag[0].append(" " + linkFlag[i]);
			linkFlag.resize(1);
			outputFlag = " " + outputFlag + " ";
			executableName += " ";
		}
		return executableName + src + linkFlag[0] + outputFlag + out;
	}
	Linker(const Object* obj) : basicModule(obj), linkFlagMerged(false) {}
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
		trace(ATTR(GREEN) "Found "
			ATTR(RESET)	"linker %s",
			linker->name.c_str());
	}
};

extern Linkers linkers;
#endif