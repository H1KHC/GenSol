#include <algorithm>
#include "modules/compiler.h"
#include "modules/config.h"
#include "modules/linker.h"

template<class T>
inline void dedumplicate(T &a) {
	for(auto it = a.rbegin(), _end = a.rend(); it != _end; ++it)
		for(auto n = a.rbegin(); n != it; ++n)
			if(*n == *it) {
				a.erase(n);
				break;
			}
}

template <class T>
inline void Merge(T &a, const T &b) {
	for(auto &obj : b) {
		bool conflicted = false;
		for(auto it = a.begin(), _end = a.end();
		  !conflicted && it != _end; ++it) {
			if(obj == *it) conflicted = true;
		}
		if(!conflicted) a.push_back(obj);
	}
}

void Compiler::merge(const Compiler* compiler) {
	if(!executableName.size())
		executableName = compiler->executableName;
	Merge(compileFlag, compiler->compileFlag);
	if(!outputFlag.length())
		outputFlag = compiler->outputFlag;
}

void Config::merge(const Config* config) {
	Merge(includeDir, config->includeDir);
	if(!distDir.length())
		distDir = config->distDir;
	if(!installPrefix.length())
		installPrefix = config->installPrefix;
	Merge(srcDir, config->srcDir);
	Merge(srcDirR, config->srcDirR);
}

void Linker::merge(const Linker* linker) {
	if(!executableName.length())
		executableName = linker->executableName;
	Merge(linkFlag, linker->linkFlag);
	if(!outputFlag.length())
		outputFlag = linker->outputFlag;
}