#include "modules/basicModule.h"
#include <regex>

basicModule::basicModule(const Object* _obj) : parsed(false), accessed(0), obj(_obj) {
	static std::regex Reg("\\W");
	if(obj->HasMember("name")) {
		auto& n = (*obj)["name"];
		if(n.GetType() != js::kStringType)
			throw ERR::OBJECT_TYPE_UNSUPPORTED;
		name = n.GetString();
		if(std::regex_match(name, Reg))
			throw ERR::OBJECT_NAME_ILLEGAL;
		if(obj->HasMember("base")) {
			auto& n = (*obj)["name"];
			if(n.GetType() == js::kStringType) {
				base.push_back(n.GetString());
				if(std::regex_match(base[0], Reg))
					throw ERR::OBJECT_NAME_ILLEGAL;
			}
			else if(n.GetType() == js::kArrayType) {
				for(int i = 0, sz = n.Size(); i < sz; ++i) {
					auto& o = n[i];
					if(o.GetType() != js::kStringType)
						throw ERR::OBJECT_TYPE_UNSUPPORTED;
					base.push_back(o.GetString());
					if(std::regex_match(base[i], Reg))
						throw ERR::OBJECT_NAME_ILLEGAL;
				}
			} else
				throw ERR::OBJECT_TYPE_UNSUPPORTED;
		} else base.push_back("global");
	} else name = "global", base.clear();
}