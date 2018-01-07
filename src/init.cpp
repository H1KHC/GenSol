#include "modules/basicModule.h"
#include "error.h"
#include <regex>

basicModule::basicModule(const Object* _obj) : parsed(false), accessed(0), obj(_obj) {
	static std::regex Reg("\\W");
	if(obj->HasMember("name")) {
		auto& n = (*obj)["name"];
		if(n.GetType() != js::kStringType)
			throw ERR::OBJECT_TYPE_UNSUPPORTED(
				"In feild \"name\", expected String"
			);
		name = n.GetString();
		if(std::regex_match(name, Reg))
			throw ERR::MODULE_NAME_ILLEGAL(
				"In feild \"name\""
			);
		if(obj->HasMember("base")) {
			auto& n = (*obj)["base"];
			if(n.GetType() == js::kStringType) {
				base.push_back(n.GetString());
				if(std::regex_match(base.front(), Reg))
					throw ERR::MODULE_NAME_ILLEGAL(
							"In feild \"base\""
						);
			}
			else if(n.GetType() == js::kArrayType) {
				for(int i = 0, sz = n.Size(); i < sz; ++i) {
					auto& o = n[i];
					if(o.GetType() != js::kStringType)
						throw ERR::OBJECT_TYPE_UNSUPPORTED(
							"In feild \"base\".#%d, expected String", i
						);
					base.push_back(o.GetString());
					if(std::regex_match(base.front(), Reg))
						throw ERR::MODULE_NAME_ILLEGAL(
							"In feild \"base\".#%d", i
						);
				}
			} else
				throw ERR::OBJECT_TYPE_UNSUPPORTED(
					"In feild \"base\""
				);
		} else if(name != "global") base.push_back("global");
		else base.clear();
	} else name = "global", base.clear();
}