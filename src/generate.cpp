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
	trace.push("Solution generate",
		ATTR(GREEN) "\nGenerating " ATTR(RESET) "makefile...");
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
		out <<" "<<target.second->config.distDir.c_str()
			<<target.first.c_str();
	out <<"\n";

	trace.pop();
	trace.log(ATTR(GREEN) "Done!" ATTR(RESET));
}

void Task::generate() {
	if(generated) return;
	trace.push("Task " + name,
		ATTR(GREEN) "Generating "
		ATTR(RESET) "task %s...", name.c_str());
	out <<"task_" <<name.c_str() <<": directories";
	for(auto& n : base)
		out <<(" task_" + n).c_str();
	for(auto &tgt : target) {
		out <<" " << tgt.ptr->config.distDir.c_str() << tgt.name.c_str();
	}
	out <<"\n\n";
	generated = true;
	trace.pop();
}

void Target::generate() {
	if(generated) return;
	directories.insert(config.distDir +
					   fs::path(name).remove_filename().string());
	trace.push("Target " + name,
		ATTR(GREEN) "Generating "
		ATTR(RESET) "target %s...", name.c_str());
	if(sources.empty() && sourcesR.empty())
		sourcesR.push_back(".*\\.cpp"),
		sourcesR.push_back(".*\\.c");
	matchFiles();
	out <<config.distDir.c_str() <<name.c_str() <<":";
	for(auto& src : sources)
		out <<" .build/" << compiler.objectFileName(src).c_str();
	out <<"\n\t" <<linker.command("$^", "$@").c_str() <<"\n\n";
	generated = true;
	trace.pop();
}

void format(char *buf) {
	std::string str(buf);
	static std::regex rgx(R"((.*\.o|\\\n ))");
	str = std::regex_replace(str, rgx, "", std::regex_constants::match_any);
	strcpy(buf, str.c_str());
}

void Target::generateSources() {
	static char buf[16384];
	if(fileGenerated) return;
	trace.push("Sources of " + name,
		ATTR(GREEN) "Generating "
		ATTR(RESET) "sources for target %s...", name.c_str());
	for(auto& src : sources) {
		trace.push("Source file " + src,
			ATTR(GREEN) "Generating "
			ATTR(RESET) "source file %s...", src.c_str());
		std::string dir = ".build/" +
							fs::path(src).remove_filename().string(),
					binName = ".build/" + compiler.objectFileName(src);
		directories.insert(dir);

		std::string&& cmd = compiler.command(src) +
							" -MM " + config.includeDirCommand();
		trace.verbose(ATTR("30") "Command: %s", cmd.c_str());
		FILE* pp = popen(cmd.c_str(), "r");
		if(!pp)
			throw ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED();
		buf[0] = 0;
		int size = fread(buf, sizeof(char), 16383, pp);
		pclose(pp);
		if(!size)
			throw ERR::SOURCE_DEPENDENCE_ANALYSIS_FAILED();
		buf[size - 1] = '\0';	//erase '\n'
		format(buf);
		out <<binName.c_str() <<buf <<"\n\t"
			<<compiler.command("$<", "$@").c_str()
			<<config.includeDirCommand().c_str() <<"\n\n";
		trace.pop();
	}
	fileGenerated = true;
	trace.pop();
}
