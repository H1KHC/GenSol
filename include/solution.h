#ifndef __SOLUTION_H__
#define __SOLUTION_H__
#include <set>
#include <string>
#include <rapidjson/document.h>
#include "error.h"

struct fileNode {
	int depth;
	std::string name;
	const fileNode *parentNode;
	bool operator < (const fileNode &node) const {
		return depth < node.depth;
	}
};

class Solution {
	bool verbose;
	std::set<fileNode> inputFiles;
	std::string outputFile, currentFile, defaultTask;
	void addIncludedFile(const std::string& file, const fileNode* parent);
	void parseObject(const rapidjson::GenericValue<rapidjson::UTF8<> >& obj, int ID);
	void analyse(const fileNode* file);
	void load();
	void check();
	//void autoremove();
	void generate();
public:
	void setDefaultTask(const std::string& str) {
		if(defaultTask.length())
			throw ERR::MULTIPLE_DEFAULT_TASK_SET("Name: \"%s\" and \"%s\" ",
				defaultTask.c_str(), str.c_str());
		defaultTask = str;
	}
	void setOutput(const std::string&);
	void addInput(const std::string&);
	void execute();

	bool isVerbose() { return verbose; }
	void setVerbose(bool v) {
		verbose = v;
	}
};

extern Solution solution;

#endif