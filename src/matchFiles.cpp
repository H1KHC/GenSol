#include <boost/filesystem.hpp>
#include <algorithm>
#include <regex>
#include "modules/task.h"
#include "trace.h"
namespace fs = boost::filesystem;

void Target::matchFiles() {
	static std::list<std::list<std::string>::iterator> toRemove;
	bool flag = false;
	toRemove.clear();
	if(fileMatched) return;
	trace.push("Target " + name,
		ATTR(GREEN) "Checking "
		ATTR(RESET) "target %s for files' existence...", name.c_str());
	for(auto file = sources.begin(),
			_end = sources.end();
			file != _end; ++file) {
		trace.log(ATTR(GREEN) "Checking "
				  ATTR(RESET) "for file %s...", file->c_str());
		flag = false;
		for(auto& dir: config.srcDir) {
			auto path = fs::current_path() / fs::path(dir);
			trace.log(ATTR(GREEN) "Searching "
					  ATTR(RESET) "in directory %s...",path.string().c_str());
			if(fs::exists(path / *file)) {
				trace.log(ATTR(GREEN) "Found!");
				*file = (path / *file).generic_string();
				file->erase(file->begin(), file->begin() + fs::current_path().string().size() + 1);
				flag = true;
				break;
			}
		}
		if(!flag) {
			toRemove.push_back(file);
			trace.log(ATTR(YELLOW)"Not found!");
		}
	}
	for(auto it = toRemove.rbegin(), _rend = toRemove.rend();
		it != _rend; ++it) {
			trace.log(ATTR(YELLOW)"Removed "
					  ATTR(RESET) "nonexistent file %s", (*it)->c_str());
			sources.erase(*it);
	}

	trace.pop();
	trace.push("Target " + name,
		  ATTR(GREEN) "Searching "
		  ATTR(RESET) "for files matching target %s...", name.c_str());
	for(auto& file : sourcesR) {
		trace.push("Regex " + file,
			ATTR(GREEN) "Matching "
			ATTR(RESET) "file regex %s...", file.c_str());
		for(auto& dir: config.srcDir) {
			std::regex R(file);
			trace.push("Directory " + dir,
				ATTR(GREEN) "Searching "
				ATTR(RESET) "in directory %s...",dir.c_str());
			for(auto&f : fs::directory_iterator(dir)) {
				auto& fn = f.path().string();
				if(std::regex_match(fn, R)) {
					trace.log(ATTR(GREEN) "Matched "
							  ATTR(RESET) "%s", fn.c_str());
					sources.push_back(
						std::regex_replace(fn, std::regex(R"((\w)\\)"), "$1/",
							std::regex_constants::match_any));
				}
			}
			trace.pop();
		}
		for(auto& dir: config.srcDirR) {
			std::regex R(file);
			trace.push("Directory " + dir,
				ATTR(GREEN) "Depth-first searching"
				ATTR(RESET) "in directory %s...",dir.c_str());
			for(auto&f : fs::recursive_directory_iterator(dir)) {
				auto& fn = f.path().string();
				if(std::regex_match(fn, R)) {
					trace.log(ATTR(GREEN) "Matched "
							  ATTR(RESET) "%s", fn.c_str());
					sources.push_back(
						std::regex_replace(fn, std::regex(R"((\w)\\)"), "$1/",
							std::regex_constants::match_any));
				}
			}
			trace.pop();
		}
		trace.pop();
	}
	sourcesR.clear();
	//deduplicate(sources);
	trace.pop();
	fileMatched = true;
}