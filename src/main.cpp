#include "error.h"
#include <cstdio>
#include "solution.h"

int main(int argc, char **argv) {
	bool flagHelp = false;
	for(int i = 1; i < argc && !flagHelp; ++i) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'h': flagHelp = true; break;
				case 'o':
					if(i + 1 == argc)
						setError(ERR::MULTIPLE_OUTPUT_FILE_SET, "%s", argv[i]);
					solution.setOutput(argv[++i]);
					break;
				//case v://Variables
				default:
					setError(ERR::UNKNOWN_OPTION, "%s", argv[i]);
			}
		} else solution.addInput(argv[i]);
	}
	if(flagHelp) {
		void help();
		help();
	} else solution.execute();
	return 0;
}