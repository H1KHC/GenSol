#ifndef __SOLUTION_H__
#define __SOLUTION_H__
#include <set>
#include <string>
#include <rapidjson/document.h>

struct fileNode {
	int depth;
	std::string name;
	const fileNode *parentNode;
	bool operator < (const fileNode &node) const {
		return depth < node.depth;
	}
};

class Solution {
	std::set<fileNode> inputFiles;
	std::string outputFile, currentFile;
	void addIncludedFile(const std::string& file, const fileNode* parent);
	void parseObject(const rapidjson::GenericValue<rapidjson::UTF8<> >& obj, int ID);
	void analyse(const fileNode* file);
	void load();
	void check();
	//void autoremove();
	void generate();
public:
	void setOutput(const std::string&);
	void addInput(const std::string&);
	void execute();
};

extern Solution solution;

#endif