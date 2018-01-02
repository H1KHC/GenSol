#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <vector>
#include <map>
#include "modules/basicModule.h"

struct Config : public basicModule {
	bool includeCommandGenerated;
	std::vector<std::string> includeDir, srcDir;
	std::string distDir, includeCommand;
	void merge(const Config*);
	void access();
	void parse();
	std::string& includeDirCommand() {
		if(!includeCommandGenerated) {
			includeCommandGenerated = true;
			includeCommand.clear();
			if(includeDir.size())
				for(auto& dir : includeDir)
					includeCommand.append(" -I"+dir);
			else includeCommand = " ";
		}
		return includeCommand;
	}
	Config(const Object* obj) :
		basicModule(obj), includeCommandGenerated(false) {}
};

class Configs {
	std::map<std::string, Config*> map;
public:
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	Config* find(const std::string& name) {
		auto it = map.find(name);
		if(it == map.end())
			throw ERR::MODULE_NOT_FOUND("Name: [config] %s", name.c_str());
		return it->second;
	}
	void insert(const Object* obj) {
		Config *config = new Config(obj);
		if(exist(config->name))
			throw ERR::MODULE_NAME_CONFLICT("Name: [config] %s", config->name);
		map[config->name] = config;
		trace(ATTR(GREEN) "Found "
			ATTR(RESET)	"config %s",
			config->name.c_str());
	}
};

extern Configs configs;
#endif