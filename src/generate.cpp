#include "solution.h"
#include "modules/task.h"
#include "out.h"
#include <set>
#include <regex>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

static std::set<std::string> directories;

void Solution::generate() {
	if(!outputFile.length()) outputFile = "makefile";
	out.init(outputFile.c_str());
	trace(ATTR(GREEN)	"\nGenerating " ATTR(RESET) "makefile...");
	trace.push();
	out <<".PHONY: directories clean";
	for(auto &task : tasks)
		out <<" task_" <<task.second->name.c_str();
	out <<"\n\n";
	if(tasks.map.size() == 1) defaultTask = tasks.map.begin()->first;
	if(defaultTask.length()) {
		out <<"default: task_" <<defaultTask.c_str() <<"\n\n";
	} else {
		out <<"default:\n\t@echo please indicate a task!\n\n";
	}
	out <<"# Tasks:\n";
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
	for(auto it = directories.begin(), _end = directories.end();
	  it != _end; ++it)
		while(it != _end && (*it == "./" || *it == ".\\"
			|| *it == "../" || *it == "..\\"))
			it = directories.erase(it);
	out <<"directories:\n\tmkdir -p";
	for(auto& dir : directories)
		out <<" "<<dir.c_str();
	out <<"\n\n";

	out <<"all:";
	for(auto& task : tasks)
		out <<" task_" <<task.first.c_str();
	out <<"\n\n";

	out <<"clean:\n\trm -rf";
	for(auto &dir : directories)
		out <<" " <<dir.c_str();
	for(auto& target : targets)
		out <<" "<<target.second->config.ptr->distDir.c_str()
			<<target.first.c_str();
	out <<"\n";

	trace.pop();
	trace(ATTR(GREEN) "Done!" ATTR(RESET));
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
		std::string&& str = " .build/" + compiler.ptr->objectFileName(src);
		if(out.pt - pt + str.length() >= 80) {
			out <<" \\\n";
			pt = out.pt;
		}
		out <<str.c_str();
	}
	out <<"\n\t" <<linker.ptr->command("$^", "$@").c_str() <<"\n\n";
	generated = true;
	trace.pop();
}

void format(char *buf) {
	static std::regex rgx("(.*\\.o:|\\\\\\n)");
	std::regex_replace(buf, rgx, "");
}

void Target::generateSources() {
	static char buf[16384];
	if(fileGenerated) return;
	trace(ATTR(GREEN) "Generating "
		ATTR(RESET) "sources for target %s...", name.c_str());
	trace.push();
	for(auto& src : sources) {
		trace(ATTR(GREEN) "Generating "
			ATTR(RESET) "source file %s...", src.c_str());
		trace.push();
		std::string dir = ".build/" +
							fs::path(src).remove_filename().string(),
					binName = ".build/" + compiler.ptr->objectFileName(src);
		directories.insert(dir);

		std::string&& cmd = compiler.ptr->command(src) +
							" -MM " + config.ptr->includeDirCommand();
		#ifdef _DEBUG
		trace(ATTR("30") "Command: %s", cmd.c_str());
		#endif
		FILE* pp = popen(cmd.c_str(), "r");
		if(!pp)
			throw ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED("Name: [target] %s",
				name.c_str());
		buf[0] = 0;
		int size = fread(buf, sizeof(char), 16383, pp);
		pclose(pp);
		if(!size)
			throw ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED("Name: [target] %s",
				name.c_str());
		buf[size - 1] = '\0';	//erase '\n'
		format(buf);
		out <<binName.c_str() <<buf <<"\n\t"
			<<compiler.ptr->command("$<").c_str() <<" -c -o $@"
			<<config.ptr->includeDirCommand().c_str() <<"\n\n";
		trace.pop();
	}
	fileGenerated = true;
	trace.pop();
}