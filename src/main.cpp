#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include "error.h"
#include "solution.h"
namespace fs = boost::filesystem;
namespace po = boost::program_options;

int main(int argc, char **argv) {
	try {
		po::options_description general("");
		general.add_options()
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
			std::cout <<"\033[32mUsage:\033[0m "
				<<fs::path(argv[0]).filename().string()
				<<" [options] [input] ...\n"
				<< general << "\n";
			return 0;
		}

		if(vm.count("input")) {
			for(auto &f : vm["input"].as<std::vector<std::string> >())
				solution.addInput(f);
		} else solution.addInput("solution.json");
		solution.setOutput(vm["output"].as<std::string>().c_str());
		solution.execute();
	} catch (const po::error &ex) {
		std::cout <<ex.what() <<std::endl;
	} catch  (const ERR &err) {
		setError(err);
	}
	return 0;
}