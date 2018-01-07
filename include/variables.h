// #include <map>
// #include <string>
// #include "modules/basicModule.h"
// #include "error.h"

// struct Variable : public basicModule {
// 	std::map<std::string, std::string> Vars;
// 	void merge(const Config*);
// 	void access();
// 	void parse();
// 	void loadData(const Object*obj);
// 	Variable(const Object* obj) :
// 		basicModule(obj) {}
// 	//Config() : basicModule() {}
// };

// struct Variables {
// 	std::map<std::string, Variable*> map;
// 	void insert(const Object* obj) {
// 		Variable *var = new Variable(obj);
// 		if(exist(var->name))
// 			throw ERR::MODULE_NAME_CONFLICT("Name: [VariableContainer] %s",
// 				config->name.c_str());
// 		map[config->name] = config;
// 		trace.log(ATTR(GREEN) "Found "
// 			ATTR(RESET)	"VariableContainer %s",
// 			config->name.c_str());
// 	}
// 	const std::string* find(const std::string& Name) {

// 	}
// };

// extern Variables variables;
