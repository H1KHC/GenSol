#include <algorithm>
#include "modules/compiler.h"
#include "modules/config.h"
#include "modules/linker.h"
#define Merge(a,b) ((a).insert((a).end(),(b).begin(),(b).end()))

void Compiler::merge(const Compiler* compiler) {
	if(!executableName.size())
		executableName = compiler->executableName;
	Merge(compileFlag, compiler->compileFlag);
	if(!outputFlag.length())
		outputFlag = compiler->outputFlag;
}

void Config::merge(const Config* config) {
	Merge(includeDir, config->includeDir);
	Merge(distDir, config->distDir);
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