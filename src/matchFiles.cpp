#include <boost/filesystem.hpp>
#include <algorithm>
#include <regex>
#include "modules/task.h"
#include "trace.h"
namespace fs = boost::filesystem;

template<typename T>
inline void deduplicate(T& c) {
	std::sort(c.begin(), c.end());
	typename T::iterator new_end = std::unique(c.begin(), c.end());
	c.erase(new_end, c.end());
}

void Target::matchFiles() {
	static std::vector<std::vector<std::string>::iterator> toRemove;
	bool flag = false;
	toRemove.clear();
	if(fileMatched) return;
	trace(ATTR(GREEN) "Checking "
		  ATTR(RESET) "target %s for files' existence...", name.c_str());
	trace.push();
	for(auto file = sources.begin(),
			_end = sources.end();
			file != _end; ++file) {
		trace(ATTR(GREEN) "Checking "
			  ATTR(RESET) "for file %s...", file->c_str());
		flag = false;
		for(auto& dir: config.ptr->srcDir) {
			auto path = fs::current_path() / fs::path(dir);
			trace(ATTR(GREEN) "Searching "
				  ATTR(RESET) "in directory %s...",path.string().c_str());
			if(fs::exists(path / *file)) {
				trace(ATTR(GREEN) "Found!");
				flag = true;
				break;
			}
		}
		if(!flag) {
			toRemove.push_back(file);
			trace(ATTR(YELLOW)"Not found!");
		}
	}
	for(auto it = toRemove.rbegin(), _rend = toRemove.rend();
		it != _rend; ++it) {
			trace(ATTR(YELLOW)"Removed "
				  ATTR(RESET) "nonexistent file %s", (*it)->c_str());
			sources.erase(*it);
	}

	trace.pop();
	trace(ATTR(GREEN) "Searching "
		  ATTR(RESET) "for files matching target %s...", name.c_str());
	trace.push();
	for(auto& file : sourcesR) {
		trace(ATTR(GREEN) "Matching "
			  ATTR(RESET) "file regex %s...", file.c_str());
		trace.push();
		for(auto& dir: config.ptr->srcDir) {
			std::regex R(file);
			trace(ATTR(GREEN) "Searching "
				ATTR(RESET) "in directory %s...",dir.c_str());
			trace.push();
			for(auto&f : fs::recursive_directory_iterator(dir)) {
				auto& fn = f.path().string();
				if(std::regex_match(fn, R)) {
					trace(ATTR(GREEN) "Matched "
						  ATTR(RESET) "%s", fn.c_str());
					sources.push_back(fn);
				}
			}
			trace.pop();
		}
		trace.pop();
	}
	sourcesR.clear();
	deduplicate(sources);
	trace.pop();
	fileMatched = true;
}