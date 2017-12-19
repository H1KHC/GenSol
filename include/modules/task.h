#ifndef __TAST_H__
#define __TAST_H__
#include <vector>
#include <map>
#include "modules/basicModule.h"
#include "modules/config.h"
#include "modules/compiler.h"
#include "modules/linker.h"
#include "modules/target.h"

struct Task : public basicModule {
	bool generated;
	std::vector<module<Target, Targets, &targets> > target;
	void parse();
	Task(const Object* obj) : basicModule(obj), generated(false) {}
	void access();
	void matchFiles(const std::vector<std::string>&);
	void generate();
};

struct Tasks {
	std::map<std::string, Task*> map;
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	void insert(const Object* obj) {
		try {
			Task *task = new Task(obj);
			if(exist(task->name))
				throw ERR::MODULE_NAME_CONFLICT;
			map[task->name] = task;
			trace(ATTR(GREEN) "Found "
				ATTR(RESET)	"task %s",
				task->name.c_str());
		} catch(ERR err) {
			setError(err);
		};
	}
	std::map<std::string, Task*>::iterator begin() { return map.begin(); }
	std::map<std::string, Task*>::iterator end() { return map.end(); }
};

extern Tasks tasks;
#endif