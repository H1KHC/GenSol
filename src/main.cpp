#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
#include <list>
#include <string>
#include <cstdio>
#include "trace.h"
#include "error.h"
#include "solution.h"
namespace po = boost::program_options;
#ifdef _DEBUG
#define DEFAULT_TRACING_DEPTH 0
#else
#define DEFAULT_TRACING_DEPTH 3
#endif
int main(int argc, char **argv) {
	try {
		po::options_description general("");
		general.add_options()
			("debug,d", "Show debug info")
			("maxdepth,m", po::value<int>()->default_value(DEFAULT_TRACING_DEPTH),
						"Max tracing depth")
			("help,h", "Show this message and exit")
			("output,o", po::value<std::string>()->default_value("makefile"), 
						"Output file name")
			("silent,s", "Don't output any log")
			("verbose,v", "Show verbose info");
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
			puts(
				"Usage: gensol [options] [input] ...\n"
				"  -d [ --debug    ]                 Show debug info\n"
				"  -m [ --maxdepth ] arg (=2)        Max tracing depth\n"
				"  -h [ --help     ]                 Show this message and exit\n"
				"  -o [ --output   ] arg (=makefile) Output file name\n"
				"  -s [ --silent   ]                 Don't output any log\n"
				"  -v [ --verbose  ]                 Show more log");
			return 0;
		}

		if(vm.count("input")) {
			for(auto &f : vm["input"].as<std::vector<std::string> >())
				solution.addInput(f);
		} else solution.addInput("solution.json");

		solution.setOutput(vm["output"].as<std::string>().c_str());
		trace.setMaxTracingDepth(vm["maxdepth"].as<int>());
		if(vm.count("silent")) trace.setOutputLevel(0, 0, 0);
		else if(vm.count("verbose")) trace.setOutputLevel(1, 1, 1);
		else if(vm.count("debug")) trace.setOutputLevel(1, 1, 0);
		else trace.setOutputLevel(1, 0, 0);
		solution.execute();
	} catch (const ERR::ERR &ex) {
		trace.leadingSpace(trace.depth);
		fputs(ex.what(), stderr);
		fputc('\n', stderr);
		while(!trace.stack.empty()) {
			trace.leadingSpace(trace.depth);
			fprintf(stderr, "In %s\n", trace.stack.top().c_str()),
			trace.pop();
		}
		return 1;
	} catch (const std::exception &ex) {
		fputs(ex.what(), stderr);
		return 1;
	}
	return 0;
}
