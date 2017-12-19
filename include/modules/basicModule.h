#ifndef __BASICMODULE_H__
#define __BASICMODULE_H__
#include <vector>
#include <string>
#include <rapidjson/document.h>
#include "error.h"
#include "trace.h"

namespace js = rapidjson;
typedef js::GenericValue<js::UTF8<> > Object;

struct basicModule {
	bool parsed;
	int accessed;
	const Object* obj;
	std::string name;
	std::vector<std::string> base;
	//virtual void access();
	//virtual void parse();

	basicModule(const Object* _obj);
};

template<class moduleType, class containerType, containerType* container>
struct module {
	bool found;
	std::string name;
	moduleType* ptr;
	module() {}
	module operator=(const std::string& str) { return *this = module(str); }
	module(const std::string& str) : found(false), name(str) {}
	module(std::string&& str) : found(false), name(str) {}
	void locate() {
		if(found) return;
		found = true;
		ptr = container->find(name);
	}
	void access() {
		locate();
		ptr->access();
	}
};

#endif