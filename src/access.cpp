#include <algorithm>
#include "modules/task.h"
#include "trace.h"

template<typename T>
inline void checkArraySuffixSlash(T& c) {
	for(auto &dir : c) {
		char ch = dir[dir.length() - 1];
		if(ch != '/' && ch != '\\')
			dir.append("/");
	}
}

void Compiler::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND(
		("In "+name).c_str()
	);
	accessed = 1;
	trace.push("Compiler "+name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "compiler %s...", name.c_str());
	parse();
	if(!base.empty()) {
		trace.log(ATTR(GREEN) "Checking "
				ATTR(RESET) "its %d base compilers (%s)...", base.size(), base.front().c_str());
		for(auto& b : base) {
			Compiler *ances = compilers.find(b);
			ances->access();
			merge(ances);
			trace.debug("Finished checking compiler %s", ances->name.c_str());
		}
		base.clear();
	}
	//deduplicate(compileFlag);
	trace.pop();
	accessed = 2;
}

void Config::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND(
		("In "+name).c_str()
	);
	accessed = 1;
	trace.push("Config " + name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "config %s...", name.c_str());
	parse();
	if(!base.empty()) {
		trace.log(ATTR(GREEN) "Checking "
				ATTR(RESET) "its %d base config (%s)...", base.size(), base.front().c_str());
		for(auto& b : base) {
			Config *ances = configs.find(b);
			ances->access();
			merge(ances);
			trace.debug("Finished checking config %s", ances->name.c_str());
		}
		base.clear();

	}
	//deduplicate(includeDir);
	checkArraySuffixSlash(includeDir);
	char ch = distDir[distDir.length() - 1];
	if(ch != '/' && ch != '\\') {
		distDir.append("/");
	}
	//deduplicate(srcDir);
	checkArraySuffixSlash(srcDir);
	trace.pop();
	accessed = 2;
}

void Linker::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND(
		("In " + name).c_str()
	);
	accessed = 1;
	trace.push("Linker " + name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "linker %s...", name.c_str());
	parse();
	if(!base.empty()) {
		trace.log(ATTR(GREEN) "Checking "
			ATTR(RESET) "its %d base linker (%s)...", base.size(), base.front().c_str());
		for(auto& b : base) {
			Linker *ances = linkers.find(b);
			trace.debug("Location: %#x", ances);
			ances->access();
			merge(ances);
			trace.debug("Finished checking linker %s", ances->name.c_str());
		}
		base.clear();
	}
	//deduplicate(linkFlag);
	trace.pop();
	accessed = 2;
}

void Target::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND(
		("In " + name).c_str()
	);
	accessed = 1;
	trace.push("Target " + name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "target %s...", name.c_str());
	parse();

	config.access();
	compiler.access();
	linker.access();
	trace.debug("Finished tool check");

	// dependency relationship handled in makefile

	//deduplicate(sourcesR);
	
	for(auto it = base.begin(), _end = base.end(); it != _end;)
		if(!targets.exist(*it)) it = base.erase(it);
		else ++it;
	
	trace.pop();
	accessed = 2;
}

void Task::access() {
	if(accessed == 2) return;
	else if(accessed == 1)
		throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND(("In "+name).c_str());
	accessed = 1;
	trace.push("Task " + name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "task %s...", name.c_str());
	parse();

	// dependency relationship handled in makefile
	for(auto it = target.begin(), _end = target.end();
	  it != _end; ++it)
		it->locate(), it->ptr->access();
		
	for(auto it = base.begin(), _end = base.end(); it != _end;)
		if(!tasks.exist(*it)) it = base.erase(it);
		else ++it;
	trace.pop();
	accessed = 2;
}