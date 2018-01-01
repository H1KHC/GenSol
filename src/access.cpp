#include <algorithm>
#include "modules/task.h"
#include "trace.h"

template<typename T>
inline void deduplicate(T& c) {
	std::sort(c.begin(), c.end());
	typename T::iterator new_end = std::unique(c.begin(), c.end());
	c.erase(new_end, c.end());
}

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
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND;
	accessed = 1;
	parse();
	trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "compiler %s...", name.c_str());
	trace.push();
	parse();
	for(auto& b : base) {
		trace(ATTR(GREEN) "Checking "
			  ATTR(RESET) "its base compiler...");
		trace.push();
		Compiler *ances = compilers.find(b);
		ances->access();
		merge(ances);
		trace.pop();
	}
	base.clear();
	deduplicate(compileFlag);
	trace.pop();
	accessed = 2;
}

void Config::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND;
	accessed = 1;
	parse();
	trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "config %s...", name.c_str());
	trace.push();
	parse();
	for(auto& b : base) {
		trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "its base config...");
		trace.push();
		Config *ances = configs.find(b);
		ances->access();
		merge(ances);
		trace.pop();
	}
	base.clear();
	deduplicate(includeDir);
	checkArraySuffixSlash(includeDir);
	char ch = distDir[distDir.length() - 1];
	if(ch != '/' && ch != '\\') {
		distDir.append("/");
	}
	deduplicate(srcDir);
	checkArraySuffixSlash(srcDir);
	trace.pop();
	accessed = 2;
}

void Linker::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND;
	accessed = 1;
	parse();
	trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "linker %s...", name.c_str());
	trace.push();
	parse();
	for(auto& b : base) {
		trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "its base linker...");
		trace.push();
		Linker *ances = linkers.find(b);
		ances->access();
		merge(ances);
		trace.pop();
	}
	base.clear();
	deduplicate(linkFlag);
	trace.pop();
	accessed = 2;
}

void Target::access() {
	if(accessed == 2) return;
	else if(accessed == 1) throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND;
	accessed = 1;
	parse();
	trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "target %s...", name.c_str());
	trace.push();
	parse();

	config.access();
	compiler.access();
	linker.access();

	// dependency relationship handled in makefile

	deduplicate(sourcesR);
	
	for(auto it = base.begin(), _end = base.end();
	  it != _end; ++it)
		if(!tasks.exist(*it)) base.erase(it);
	
	trace.pop();
	accessed = 2;
}

void Task::access() {
	if(accessed == 2) return;
	else if(accessed == 1)
		throw ERR::MODULE_CYCLIC_DEPENDENCE_FOUND;
	accessed = 1;
	trace(ATTR(GREEN) "Checking "
		ATTR(RESET) "task %s...", name.c_str());
	trace.push();
	parse();

	// dependency relationship handled in makefile
	for(auto it = target.begin(), _end = target.end();
	  it != _end; ++it)
		it->locate(), it->ptr->access();
		
	for(auto it = base.begin(), _end = base.end();
	  it != _end; ++it)
		if(!tasks.exist(*it)) base.erase(it);
	trace.pop();
	accessed = 2;
}