#ifndef __BASICMODULE_H__
#define __BASICMODULE_H__
#include <rapidjson/document.h>
#include <list>
#include <string>

namespace js = rapidjson;
typedef rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> > Object;

struct basicModule {
	bool parsed;
	int accessed;
	const Object* obj;
	std::string name;
	std::list<std::string> base;
	//virtual void access();
	//virtual void parse();

	basicModule(const Object* _obj);
	basicModule() : parsed(false), accessed(0), obj(nullptr), name(), base() {}
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