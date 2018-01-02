#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <cstdio>
#include "trace.h"
#include "error.h"
#include "solution.h"
namespace po = boost::program_options;
#ifdef _DEBUG
#define DEFAULT_TRACING_DEPTH 0
#else
#define DEFAULT_TRACING_DEPTH 2
#endif
int main(int argc, char **argv) {
	try {
		po::options_description general("");
		general.add_options()
			("depth,d", po::value<int>()->default_value(DEFAULT_TRACING_DEPTH),
						"Max tracing depth")
			("help,h", "Show this message and exit")
			("output,o", po::value<std::string>()->default_value("makefile"), 
						"Output file name");
		po::options_description hidden("Hidden options");
		hidden.add_options()
			("input,i", po::value< std::vector<std::string> >()->composing(),
						"Input filename");
		po::options_description options;
		options.add(general).add(hidden);
		po::positional_options_description p;
		p.add("input", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
			options(options).positional(p).run(), vm);
		po::notify(vm);

		if (vm.count("help")) {
			puts("Usage: gensol [options] [input] ...\n"
				"  -d [ --depth  ] arg (=2)        Max tracing depth\n"
				"  -h [ --help   ]                 Show this message and exit\n"
				"  -o [ --output ] arg (=makefile) Output file name");
			return 0;
		}

		if(vm.count("input")) {
			for(auto &f : vm["input"].as<std::vector<std::string> >())
				solution.addInput(f);
		} else solution.addInput("solution.json");
		solution.setOutput(vm["output"].as<std::string>().c_str());
		trace.setMaxTracingDepth(vm["depth"].as<int>());
		solution.execute();
	} catch (const exception &ex) {
		puts(ex.what());
	}
	return 0;
}