#include "solution.h"
#include "modules/task.h"
#include "out.h"
#include <set>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

static std::set<std::string> addedSources, directories;

void Solution::generate() {
	if(!outputFile.length()) outputFile = "makefile";
	out.init(outputFile.c_str());
	trace(ATTR(GREEN)	"\nGenerating "
		ATTR(RESET) "makefile...");
	trace.push();
	out <<".PHONY: directories clean";
	for(auto &task : tasks)
		out <<" task_" <<task.second->name.c_str();
	out <<"\n\n# Tasks:\n";
	for(auto &task : tasks)
		task.second->generate();
	out <<"# Targets:\n";
	for(auto &target : targets)
		if(target.second->parsed)
			target.second->generate();
	out <<"# Sources:\n";
	for(auto &target : targets)
		if(target.second->parsed)
			target.second->generateSources();
	out <<"# Others:\n";

	directories.insert(".build/");
	out <<"directories:\n\tmkdir -p";
	for(auto& dir : directories) {
		out <<" "<<dir.c_str();
	}
	out <<"\n";

	out <<"clean:\n\trm -rf";
	for(auto &dir : directories)
		out <<" " <<dir.c_str();
	out <<'\n';

	trace.pop();
	trace(ATTR(GREEN) "Done!");
}

void Task::generate() {
	if(generated) return;
	trace(ATTR(GREEN) "Generating "
		  ATTR(RESET) "task %s...", name.c_str());
	trace.push();
	int pt = out.pt;
	out <<"task_" <<name.c_str() <<": directories";
	for(auto& n : base) {
		std::string&&str = " task_" + n;
		if(out.pt + str.length() - pt >= 80) out <<" \\\n", pt = out.pt;
		out <<str.c_str();
	}
	for(int i = 0, sz = target.size(); i < sz; ++i) {
		std::string&&str = " " + target[i].ptr->config.ptr->distDir +
						   target[i].name;
		if(out.pt + str.length() - pt >= 80) out <<" \\\n", pt = out.pt;
		out <<str.c_str();
	}
	out <<"\n\n";
	generated = true;
	trace.pop();
	trace(ATTR(GREEN) "Done!");
}

void Target::generate() {
	if(generated) return;
	directories.insert(config.ptr->distDir +
					   fs::path(name).remove_filename().string());
	trace(ATTR(GREEN) "Generating "
		  ATTR(RESET) "target %s...", name.c_str());
	trace.push();
	matchFiles();
	int pt = out.pt;
	out <<config.ptr->distDir.c_str() <<name.c_str() <<":";
	for(auto& src : sources) {
		std::string&& str = (" .build/" + src) + ".o";
		if(out.pt - pt + str.length() >= 80) {
			out <<" \\\n";
			pt = out.pt;
		}
		out <<str.c_str();
	}
	out <<"\n\t" <<linker.ptr->command("$^", "$@").c_str() <<"\n\n";
	generated = true;
	trace.pop();
	trace(ATTR(GREEN) "Done!");
}

void Target::generateSources() {
	static char buf[16384];
	if(fileGenerated) return;
	trace(ATTR(GREEN) "Generating "
		ATTR(RESET) "sources for target %s...", name.c_str());
	trace.push();
	for(auto& src : sources) {
		if(!addedSources.count(src)) {
			addedSources.insert(src);
			trace(ATTR(GREEN) "Checking "
				ATTR(RESET) "file %s for dependence...", src.c_str());
			trace.push();
			std::string dir = ".build/" +
							  fs::path(src).remove_filename().string(),
						binName = ".build/" + src + ".o";
			directories.insert(dir);

			std::string&& cmd = compiler.ptr->command(src) +
								" -MM " + config.ptr->includeDirCommand();
			#ifdef _DEBUG
			trace(ATTR("30") "Command: %s", cmd.c_str());
			#endif
			FILE* pp = popen(cmd.c_str(), "r");
			if(!pp)
				setError(ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED);
			buf[0] = 0;
			for(int i = 1; i <= 10000000; ++i) {
				int j = i;
				i = j;
			}
			int size = fread(buf, sizeof(char), 16383, pp);
			pclose(pp);
			buf[size - 1] = '\0';	//erase '\n'
			char *pt = strchr(buf, ':');
			if(!pt)
				setError(ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED);
			out <<binName.c_str() <<pt <<"\n\t"
				<<compiler.ptr->command("$<").c_str() <<" -c -o $@"
				<<config.ptr->includeDirCommand().c_str() <<"\n\n";
			trace.pop();
			trace(ATTR(GREEN) "Done!");
		}
	}
	fileGenerated = true;
	trace.pop();
	trace(ATTR(GREEN) "Done!");
}