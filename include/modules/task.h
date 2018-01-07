#ifndef __TAST_H__
#define __TAST_H__
#include "modules/compiler.h"
#include "modules/config.h"
#include "modules/linker.h"
#include "modules/target.h"

struct Task : public basicModule {
	bool generated;
	std::list<module<Target, Targets, &targets> > target;
	void parse();
	void loadData(const Object*obj);
	Task(const Object* obj) : basicModule(obj), generated(false) {}
	void access();
	void generate();
};

struct Tasks {
	std::map<std::string, Task*> map;
	bool exist(const std::string& name) {
		return map.find(name) != map.end();
	}
	void insert(const Object* obj) {
		Task *task = new Task(obj);
		if(exist(task->name))
			throw ERR::MODULE_NAME_CONFLICT("Name: [task] %s",
				task->name.c_str());
		map[task->name] = task;
		trace.log(ATTR(GREEN) "Found "
			ATTR(RESET)	"task %s",
			task->name.c_str());
	}
	std::map<std::string, Task*>::iterator begin() { return map.begin(); }
	std::map<std::string, Task*>::iterator end() { return map.end(); }
};

extern Tasks tasks;
#endif