#include "trace.h"
#include "solution.h"
#include "error.h"
#include "modules/task.h"
#include "out.h"

Solution solution;

void Solution::setOutput(const std::string& file) {
	static bool set = false;
	if(set) throw ERR::MULTIPLE_OUTPUT_FILE_SET();
	set = true;
	outputFile = file;
}

void Solution::addIncludedFile(const std::string& file, const fileNode* parent) {
	inputFiles.insert(fileNode{ parent->depth + 1, file, parent });
}

void Solution::addInput(const std::string& file) {
	inputFiles.insert(fileNode{ 0, file, nullptr });
}

void Solution::check() {
	trace.push("Solution check",
		ATTR(GREEN)"\nChecking "
		ATTR(RESET) "tasks...");
	for(auto &task : tasks)
		task.second->access();
	trace.pop();
	trace.log(ATTR(GREEN) "Done!" ATTR(RESET));
}

void Solution::execute() {
	load();
	check();
	//autoremove();
	generate();
}